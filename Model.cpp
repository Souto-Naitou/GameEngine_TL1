#include "Model.h"

#include <structs.h>
#include <DX12Helper.h>
#include <Features/Model/Helper/ModelHelper.h>
#include <Features/Object3d/Object3dSystem.h>

const std::string Model::kDefaultDirectoryPath = "resources";

void Model::Initialize(const std::string& _filePath)
{
    /// 必要なインスタンスを取得
    pDx12_ = _system->GetDx12();
    device_ = pDx12_->GetDevice();


    /// モデルデータを読み込む
    modelData_ = ModelHelper::LoadObjFile(kDefaultDirectoryPath, _filepath);


    /// 頂点リソースを作成
    CreateVertexResource();

    /// マテリアルリソースを作成
    CreateMaterialResource();

}

void Model::Draw()
{
    // 頂点バッファを設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    // マテリアルCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

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


void Model::CreateMaterialResource()
{
    /// マテリアルリソースを作成
    materialResource_ = DX12Helper::CreateBufferResource(device_, sizeof(Material));
    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
    /// マテリアルデータを初期化
    materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_->enableLighting = false;
    materialData_->uvTransform = Matrix4x4::Identity();
}