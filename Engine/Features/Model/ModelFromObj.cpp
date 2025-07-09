#include "./ModelFromObj.h"

#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Features/Model/Helper/ModelHelper.h>
#include <Features/Object3d/Object3dSystem.h>
#include <Core/DirectX12/TextureManager.h>
#include <Features/Model/ModelManager.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <filesystem>

void ModelFromObj::Initialize()
{

}

void ModelFromObj::Update()
{
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
}

void ModelFromObj::CreateGPUResource()
{
    /// 頂点リソースを作成
    _CreateVertexResource();

    /// テクスチャを読み込む
    _LoadModelTexture();

    // フラグを立てる
    isReadyDraw_ = true;
}

void ModelFromObj::Clone(IModel* _src)
{
    if (_src == nullptr) return;
    
    ModelFromObj* src = dynamic_cast<ModelFromObj*>(_src);
    if (src == nullptr) return;

    if (src->IsEndLoading() == false)
    {
        // Updateで読み込みが終わるまで待つ
        // データのコピーはUpdateで行われる
        pCloneSrc_ = src;
        return;
    }

    this->_CopyFrom(src);
}

void ModelFromObj::Draw()
{
    if (isReadyDraw_ == false) return;

    // 頂点バッファを設定
    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
    // SRVの設定
    commandList_->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
    // 描画！（DrawCall/ドローコール）
    commandList_->DrawInstanced(static_cast<uint32_t>(modelData_.vertices.size()), 1, 0, 0);
}

ModelFromObj::~ModelFromObj()
{
    OutputDebugStringA("Model Destructor\n");
}

void ModelFromObj::_CreateVertexResource()
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

void ModelFromObj::_LoadModelTexture()
{
    std::string filePath = modelData_.materialData.textureFilePath;

    if (!std::filesystem::exists(modelData_.materialData.textureFilePath))
    {
        DebugManager::GetInstance()->PushLog("[Warning] The model's texture could not be loaded so white1x1.png is loaded instead : " + modelData_.materialData.textureFilePath + "\n");
        Logger::GetInstance()->LogWarning(
            "Model",
            "LoadModelTexture",
            "Could'nt load so white1x1.png is loaded instead. path: " + modelData_.materialData.textureFilePath
        );

        filePath = "white1x1.png";
    }

    TextureManager* textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture(filePath);
    textureSrvHandleGPU_ = textureManager->GetSrvHandleGPU(filePath);
}

void ModelFromObj::_CopyFrom(ModelFromObj* _pCopySrc)
{
    this->pDx12_ = _pCopySrc->pDx12_;
    // モデルデータをコピー
    this->modelData_ = _pCopySrc->modelData_;
    // テクスチャのSRVハンドルをコピー
    this->textureSrvHandleGPU_ = _pCopySrc->textureSrvHandleGPU_;
    // 頂点リソースを作成
    this->_CreateVertexResource();

    isReadyDraw_ = true;
}

ModelData* ModelFromObj::GetModelData()
{
    return &modelData_;
}

size_t ModelFromObj::GetVertexCount() const
{
    return modelData_.vertices.size();
}

D3D12_VERTEX_BUFFER_VIEW ModelFromObj::GetVertexBufferView() const
{
    return vertexBufferView_;
}

bool ModelFromObj::IsEndLoading() const
{
    return isReadyDraw_; 
}

D3D12_GPU_DESCRIPTOR_HANDLE ModelFromObj::GetTextureSrvHandle() const
{
    return textureSrvHandleGPU_; 
}

int ModelFromObj::ChangeTexture(const std::string& _filepath)
{
    // Note: もしUpload関数が呼ばれる前にこの関数を呼ぶとここで変更したテクスチャハンドルが
    //       mtlファイルに記述されるテクスチャのハンドルに置き換えられてしまうため
    if (!isReadyDraw_) return -1;

    auto tm = TextureManager::GetInstance();
    tm->LoadTexture(_filepath);
    textureSrvHandleGPU_ = tm->GetSrvHandleGPU(_filepath);

    return 0;
}

void ModelFromObj::SetCommandList(ID3D12GraphicsCommandList* _cl)
{
    commandList_ = _cl;
}

void ModelFromObj::ChangeTexture(D3D12_GPU_DESCRIPTOR_HANDLE _texSrvHnd)
{
    textureSrvHandleGPU_ = _texSrvHnd;
}
