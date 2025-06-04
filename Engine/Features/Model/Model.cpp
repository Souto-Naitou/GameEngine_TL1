#include "Model.h"

#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Features/Model/Helper/ModelHelper.h>
#include <Features/Object3d/Object3dSystem.h>
#include <Core/DirectX12/TextureManager.h>
#include <Features/Model/ModelManager.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <filesystem>

void Model::Initialize(const std::string& _filePath)
{
    /// 必要なインスタンスを取得
    pDx12_ = DirectX12::GetInstance();
    device_ = pDx12_->GetDevice();

    filePath_ = _filePath;

    /// モデルデータを読み込む
    th_LoadObjectFile_ = std::make_unique<std::thread>([&]()
    {
        directoryPath_ = ModelManager::GetInstance()->GetDirectoryPath(filePath_);
        modelData_ = ModelHelper::LoadObjFile(directoryPath_, filePath_, texturePath_);
        ModelManager::GetInstance()->InqueueUpload(this);
        Logger::GetInstance()->LogInfo(
            "Model",
            "LoadModel",
            "Succeed : " + directoryPath_ + filePath_
        );
    });
}

void Model::Initialize(const std::string& _modelPath, const std::string& _texturePath)
{
    texturePath_ = _texturePath;
    Initialize(_modelPath);
}

void Model::Update()
{
}

void Model::Draw(ID3D12GraphicsCommandList* _commandList)
{
    if (th_LoadObjectFile_->joinable()) return;

    // 頂点バッファを設定
    _commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    // SRVの設定
    _commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
    // 描画！（DrawCall/ドローコール）
    _commandList->DrawInstanced(static_cast<uint32_t>(modelData_.vertices.size()), 1, 0, 0);
}

Model::~Model()
{
    OutputDebugStringA("Model Destructor\n");
    if (th_LoadObjectFile_->joinable()) th_LoadObjectFile_->join();
}

void Model::CreateVertexResource()
{
    /// 頂点リソースを作成
    vertexResource_ = DX12Helper::CreateBufferResource(device_, sizeof(VertexData) * modelData_.vertices.size());
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

    /// 頂点データを初期化
    std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

    /// 頂点バッファービューを初期化
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = static_cast<uint32_t>(sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Model::LoadModelTexture()
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

void Model::Upload()
{
    if (th_LoadObjectFile_ && th_LoadObjectFile_->joinable())
    {
        th_LoadObjectFile_->join();
    }

    /// 頂点リソースを作成
    CreateVertexResource();

    /// テクスチャを読み込む
    LoadModelTexture();

    isUploaded_ = true;
}

ModelData* Model::GetModelData()
{
    return &modelData_;
}

D3D12_VERTEX_BUFFER_VIEW Model::GetVertexBufferView() const
{
    return vertexBufferView_;
}

bool Model::IsUploaded() const
{
    return isUploaded_; 
}

D3D12_GPU_DESCRIPTOR_HANDLE Model::GetTextureSrvHandleGPU() const
{
    return textureSrvHandleGPU_; 
}

int Model::ChangeTexture(const std::string& _filepath)
{
    // Note: もしUpload関数が呼ばれる前にこの関数を呼ぶとここで変更したテクスチャハンドルが
    //       mtlファイルに記述されるテクスチャのハンドルに置き換えられてしまうため
    if (!isUploaded_) return -1;

    auto tm = TextureManager::GetInstance();
    tm->LoadTexture(_filepath);
    textureSrvHandleGPU_ = tm->GetSrvHandleGPU(_filepath);

    return 0;
}
