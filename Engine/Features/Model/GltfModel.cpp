#include "./GltfModel.h"
#include <DebugTools/DebugManager/DebugManager.h>
#include <Core/DirectX12/TextureManager.h>
#include <Features/Model/Helper/AnimationHelper.h>
#include <cmath>
#include <stdexcept>

GltfModel::~GltfModel()
{
    if (is_called_finalize_ == false)
    {
        assert(false && "GltfModel::Finalize() must be called before destruction.");
    }
}

void GltfModel::Initialize()
{
    isOverwroteTexture_ = false;
    timer_.Reset();
    timer_.Start();
}

void GltfModel::Finalize()
{
    SRVManager::GetInstance()->Deallocate(wellSRVIndex_);

    is_called_finalize_ = true;
}

void GltfModel::Update()
{
    // 遅延クローン処理
    if (pCloneSrc_ != nullptr)
    {
        // クローン元のモデルが読み込み完了している場合
        if (pCloneSrc_->IsEndLoading())
        {
            // クローン元からデータをコピー
            this->_CopyFrom(pCloneSrc_);
            pCloneSrc_ = nullptr; // クローン元をリセット
        }
    }

    //this->_UpdateLocalMatrixByAnimation();

    this->_ApplyAnimationToSkeleton();

    this->_UpdateSkeleton();

    this->_UpdateSkinCluster();
}

void GltfModel::Draw(ID3D12GraphicsCommandList* _cl)
{
    if (isReadyDraw_ == false) return;

    // 頂点バッファを設定
    D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
        vertexBufferView_,                  // 通常の頂点バッファ
        skinCluster_.influenceBufferView    // スキンクラスターの頂点バッファ
    };

    _cl->IASetVertexBuffers(0, 2, vbvs);
    // インデックスバッファを設定
    _cl->IASetIndexBuffer(&indexBufferView_);
    // SRVの設定
    _cl->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
    // Wellリソースを設定
    _cl->SetGraphicsRootDescriptorTable(8, wellSrvHandleGPU_);
    // 描画！（DrawCall/ドローコール）
    _cl->DrawIndexedInstanced(
        static_cast<UINT>(modelData_.indices.size()), // インデックス数
        1, // インスタンス数
        0, // インデックスバッファの開始オフセット
        0, // 頂点バッファの開始オフセット
        0  // グループID（インスタンス化描画用）
    );
}

void GltfModel::ChangeTexture(D3D12_GPU_DESCRIPTOR_HANDLE _texSrvHnd)
{
    textureSrvHandleGPU_ = _texSrvHnd;
    isOverwroteTexture_ = true;
}

void GltfModel::CreateGPUResource()
{
    /// 頂点リソースを作成
    _CreateVertexResource();

    /// インデックスリソースを作成
    _CreateIndexResource();

    /// テクスチャを読み込む
    _LoadModelTexture();

    /// ウェルリソースを作成
    _CreateWellResource();

    // フラグを立てる
    isReadyDraw_ = true;
}

bool GltfModel::IsEndLoading() const
{
    return isReadyDraw_; 
}

void GltfModel::Clone(IModel* _src)
{
    if (_src == nullptr) return;

    isOverwroteTexture_ = false;

    // クローン元がGltfModelでない場合はエラー
    GltfModel* pSrc = dynamic_cast<GltfModel*>(_src);
    if (!pSrc)
    {
        throw std::runtime_error("GltfModel::Clone: Source model is not a GltfModel.");
        return;
    }

    if (pSrc->IsEndLoading() == false)
    {
        // Updateで読み込みが終わるまで待つ
        // データのコピーはUpdateで行われる
        pCloneSrc_ = pSrc;
        return;
    }

    this->_CopyFrom(pSrc);
}

D3D12_VERTEX_BUFFER_VIEW GltfModel::GetVertexBufferView() const
{
    return vertexBufferView_;
}

size_t GltfModel::GetVertexCount() const
{
    return modelData_.vertices.size();
}

D3D12_GPU_DESCRIPTOR_HANDLE GltfModel::GetTextureSrvHandle() const
{
    return textureSrvHandleGPU_; 
}

ModelData* GltfModel::GetModelData()
{
    return &modelData_;
}

Animation* GltfModel::GetAnimationData()
{
    return &animationData_;
}

Skeleton* GltfModel::GetSkeleton()
{
    return &skeleton_;
}

SkinCluster* GltfModel::GetSkinCluster()
{
    return &skinCluster_;
}

void GltfModel::_CreateVertexResource()
{
    /// 頂点リソースを作成
    vertexResource_ = DX12Helper::CreateBufferResource(pDx12_->GetDevice(), sizeof(VertexData) * modelData_.vertices.size());
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

    /// 頂点データを初期化
    std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

    /// 頂点バッファービューを初期化
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = static_cast<uint32_t>(sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void GltfModel::_CreateIndexResource()
{
    /// インデックスリソースを作成
    indexResource_ = DX12Helper::CreateBufferResource(pDx12_->GetDevice(), sizeof(uint32_t) * modelData_.indices.size());
    indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

    /// インデックスデータを初期化
    std::memcpy(indexData_, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());

    /// インデックスバッファービューを初期化
    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = static_cast<uint32_t>(sizeof(uint32_t) * modelData_.indices.size());
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}


void GltfModel::_LoadModelTexture()
{
    if (isOverwroteTexture_)
    {
        // テクスチャを上書きした場合は、マテリアルファイル記述のテクスチャを使用しない
        return;
    }

    std::string filePath = modelData_.material.textureFilePath;

    if (!std::filesystem::exists(modelData_.material.textureFilePath))
    {
        DebugManager::GetInstance()->PushLog("[Warning] The model's texture could not be loaded so white1x1.png is loaded instead : " + modelData_.material.textureFilePath + "\n");
        Logger::GetInstance()->LogWarning(
            "Model",
            "LoadModelTexture",
            "Could'nt load so white1x1.png is loaded instead. path: " + modelData_.material.textureFilePath
        );

        filePath = "white1x1.png";
    }

    TextureManager* textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture(filePath);
    textureSrvHandleGPU_ = textureManager->GetSrvHandleGPU(filePath);

}

void GltfModel::_CopyFrom(GltfModel* _pCopySrc)
{
    this->pDx12_ = _pCopySrc->pDx12_;
    // モデルデータをコピー
    this->modelData_ = _pCopySrc->modelData_;
    this->animationData_ = _pCopySrc->animationData_;
    // テクスチャのSRVハンドルをコピー
    // NOTE: クローン元のテクスチャを上書きしていない場合に限る
    if (!isOverwroteTexture_)
    {
        this->textureSrvHandleGPU_ = _pCopySrc->textureSrvHandleGPU_;
    }
    this->skeleton_ = _pCopySrc->skeleton_;
    this->skinCluster_ = _pCopySrc->skinCluster_;
    // リソースを作成
    this->_CreateVertexResource();
    this->_CreateIndexResource();
    this->_CreateWellResource();

    isReadyDraw_ = true;
}

void GltfModel::_UpdateLocalMatrixByAnimation()
{
    if (timer_.GetNow<float>() >= animationData_.duration)
    {
        timer_.Reset();
        timer_.Start();
    }

    // RootAnimationの取得
    NodeAnimation& rootNodeAnim = animationData_.nodeAnimations[modelData_.rootNode.name];
    Vector3 scale = Helper::Animation::CalculateValue(rootNodeAnim.scale, timer_.GetNow<float>());
    Quaternion rotation = Helper::Animation::CalculateValue(rootNodeAnim.rotation, timer_.GetNow<float>());
    Vector3 translation = Helper::Animation::CalculateValue(rootNodeAnim.translate, timer_.GetNow<float>());
    Matrix4x4 localMatrix = Matrix4x4::AffineMatrix(scale, rotation, translation);

    modelData_.rootNode.localMatrix = localMatrix;
}

void GltfModel::_UpdateSkeleton()
{
    for (Joint& joint : skeleton_.GetSkeletonData().joints)
    {
        JointData& jointData = joint.GetJointData();

        jointData.localMatrix = Matrix4x4::AffineMatrix(
            jointData.transform.scale,
            jointData.transform.rotate,
            jointData.transform.translate
        );

        if (jointData.parentIndex)
        {
            // 親ジョイントが存在するなら親の行列を掛ける
            const Matrix4x4& parentSkeletonSpaceMatrix = skeleton_.GetSkeletonData().joints[*jointData.parentIndex].GetJointData().skeletonSpaceMatrix;
            jointData.skeletonSpaceMatrix = jointData.localMatrix * parentSkeletonSpaceMatrix;
        }
        else
        {
            // 親ジョイントがいないのでlocalMatrixとskeletonSpaceMatrixは同じ
            jointData.skeletonSpaceMatrix = jointData.localMatrix;
        }
    }

    skeleton_.Update();
}

void GltfModel::_UpdateSkinCluster()
{
    auto& skeletonData = skeleton_.GetSkeletonData();
    for (size_t jointIndex = 0; jointIndex < skeletonData.joints.size(); ++jointIndex)
    {
        if (jointIndex >= skinCluster_.inverseBindPoseMatrices.size())
        {
            // Jointの数がSkinClusterの逆バインドポーズマトリックスの数を超えた場合はエラー
            throw std::runtime_error("Joint index exceeds the size of inverseBindPoseMatrices in SkinCluster.");
        }
        skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix = 
            skinCluster_.inverseBindPoseMatrices[jointIndex] * skeletonData.joints[jointIndex].GetJointData().skeletonSpaceMatrix;
        skinCluster_.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix = 
            skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix.Inverse().Transpose();
    }
}

void GltfModel::_ApplyAnimationToSkeleton()
{
    if (timer_.GetNow<float>() >= animationData_.duration)
    {
        timer_.Reset();
        timer_.Start();
    }

    SkeletonData& skeletonData = skeleton_.GetSkeletonData();
    for (Joint& joint : skeletonData.joints)
    {
        JointData& jointData = joint.GetJointData();
        if (auto it = animationData_.nodeAnimations.find(jointData.name); it != animationData_.nodeAnimations.end())
        {
            const NodeAnimation& rootNodeAnimation = (*it).second;
            // アニメーションの適用
            jointData.transform.translate = Helper::Animation::CalculateValue(rootNodeAnimation.translate, timer_.GetNow<float>());
            jointData.transform.rotate = Helper::Animation::CalculateValue(rootNodeAnimation.rotation, timer_.GetNow<float>());
            jointData.transform.scale = Helper::Animation::CalculateValue(rootNodeAnimation.scale, timer_.GetNow<float>());
        }
    }
}

void GltfModel::_CreateWellResource()
{
    uint32_t stride = sizeof(WellForGPU);
    uint32_t countJoints = static_cast<uint32_t>(skeleton_.GetSkeletonData().joints.size());

    wellResource_ = DX12Helper::CreateBufferResource(pDx12_->GetDevice(), stride * countJoints);
    wellResource_->Map(0, nullptr, reinterpret_cast<void**>(&skinCluster_.mappedPalette));

    // 初期化いる？
    //wellForGPUData_->skeletonSpaceMatrix = Matrix4x4::Identity();
    //wellForGPUData_->skeletonSpaceInverseTransposeMatrix = Matrix4x4::Identity();

    // SRVの作成
    wellSRVIndex_ = SRVManager::GetInstance()->Allocate();
    SRVManager::GetInstance()->CreateForStructuredBuffer(wellSRVIndex_, wellResource_.Get(), countJoints, stride);
    wellSrvHandleGPU_ = SRVManager::GetInstance()->GetGPUDescriptorHandle(wellSRVIndex_);
}