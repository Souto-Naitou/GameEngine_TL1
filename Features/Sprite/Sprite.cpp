#include "Sprite.h"

#include "Features/Sprite/SpriteSystem.h"
#include "DX12Helper.h"
#include <Matrix4x4.h>


Sprite::Sprite()
{

}


Sprite::~Sprite()
{
}


void Sprite::Initialize(SpriteSystem* _spriteSystem, std::string _filepath, Vector3 _scale, Vector3 _rotate, Vector3 _transform)
{
    pSpriteSystem_ = _spriteSystem;
    pDx12_ = pSpriteSystem_->GetDx12();
    device_ = pDx12_->GetDevice();


    /// Create BufferResource
    // 頂点リソースを作成する
    CreateVertexResource();

    // インデックスリソースを作成する
    CreateIndexResource();


    /// Create View
    // 頂点バッファビューを作成する
    CreateVertexBufferView();

    // インデックスバッファビューを作成する
    CreateIndexBufferView();


    /// Map
    // 頂点データを設定する
    MapVertexData();

    // インデックスデータを設定する
    MapIndexData();

    /// Material
    // マテリアルリソースを作成する
    CreateMaterialResource();

    // 座標変換行列を表すデータを作成する
    CreateTransformationMatrixResource();

    std::string texturePath;
    std::string textureName = _filepath;
    texturePath = "Resources/" + textureName;

    ID3D12DescriptorHeap* srvDescriptorHeap = pDx12_->GetSRVDescriptorHeap();
    uint32_t kDescriptorSizeSRV = pDx12_->GetDescriptorSizeSRV();
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& textureResources = pDx12_->GetTextureResources();
    DX12Helper::CreateNewTexture(device_, srvDescriptorHeap, kDescriptorSizeSRV, texturePath.c_str(), textureResources);

    transform_.scale = _scale;
    transform_.rotate = _rotate;
    transform_.translate = _transform;
}


void Sprite::Update()
{
    uint32_t clientWidth = pDx12_->GetClientWidth();
    uint32_t clientHeight = pDx12_->GetClientHeight();

    // 左下
    vertexData_[0].position = { 0.0f, 1.0f, 0.0f, 1.0f };
    vertexData_[0].texcoord = { 0.0f, 1.0f };
    vertexData_[0].normal = { 0.0f, 0.0f, -1.0f };
    // 左上
    vertexData_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };
    vertexData_[1].texcoord = { 0.0f, 0.0f };
    vertexData_[1].normal = { 0.0f, 0.0f, -1.0f };
    // 右下
    vertexData_[2].position = { 1.0f, 1.0f, 0.0f, 1.0f };
    vertexData_[2].texcoord = { 1.0f, 1.0f };
    vertexData_[2].normal = { 0.0f, 0.0f, -1.0f };
    // 右上
    vertexData_[3].position = { 1.0f, 0.0f, 0.0f, 1.0f };
    vertexData_[3].texcoord = { 1.0f, 0.0f };
    vertexData_[3].normal = { 0.0f, 0.0f, -1.0f };


    transform_.scale = { size_.x, size_.y, 1.0f };
    transform_.rotate = { 0.0f, 0.0f, rotate_ };
    transform_.translate = translate_;

    // WVPMatrixの作成
    Matrix4x4 worldMatrix = Matrix4x4::AffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
    Matrix4x4 viewMatrix = Matrix4x4::Identity();
    Matrix4x4 projectionMatrix = Matrix4x4::OrthographicMatrix(0.0f, 0.0f, float(clientWidth), float(clientHeight), 0.0f, 100.0f);
    Matrix4x4 WVPMatrix = worldMatrix * viewMatrix * projectionMatrix;
    transformationMatrixData_->wvp = WVPMatrix;

    /// UVTransformMatrixの更新 (まだ使えない)
    uvTransformMatrix_ = Matrix4x4::ScaleMatrix(uvTransform_.scale);
    uvTransformMatrix_ *= Matrix4x4::RotateZMatrix(uvTransform_.rotate.z);
    uvTransformMatrix_ *= Matrix4x4::TranslateMatrix(uvTransform_.translate);
}


void Sprite::Draw()
{
    ID3D12GraphicsCommandList* commandList = pDx12_->GetCommandList();
    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> textureSrvHandleGPUs = pDx12_->GetSRVHandlesGPUList();
    // VBVの設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    // IBVの設定
    commandList->IASetIndexBuffer(&indexBufferView_);

    // マテリアルリソースの設定
    commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    // TransformationMatrixCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
    // SRVの設定
    commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPUs[0]);

    // 描画！（DrawCall/ドローコール）
    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

}


/// 頂点リソースを作成する
void Sprite::CreateVertexResource()
{
    vertexResource_ = DX12Helper::CreateBufferResource(device_, sizeof(VertexData) * 4);
}


/// インデックスリソースを作成
void Sprite::CreateIndexResource()
{
    indexResource_ = DX12Helper::CreateBufferResource(device_, sizeof(uint32_t) * 6);
}


/// 頂点バッファビューを作成する
void Sprite::CreateVertexBufferView()
{
    // リソースの先頭のアドレスから使う
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    // 使用するリソースのサイズは頂点６つ分のサイズ
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
    // 1頂点あたりのサイズ
    vertexBufferView_.StrideInBytes = sizeof(VertexData);
}


/// IBVの作成
void Sprite::CreateIndexBufferView()
{
    // リソースの先頭のアドレスから使用する
    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    // 使用するリソースのサイズはインデックス6つ分のサイズ
    indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
    // インデックスはint32_tとする
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

}


/// 頂点データを設定する
void Sprite::MapVertexData()
{
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}


/// インデックスデータを設定する
void Sprite::MapIndexData()
{
    indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
    indexData_[0] = 0;	indexData_[1] = 1;	indexData_[2] = 2;
    indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;
}


/// マテリアルリソースを作成する
void Sprite::CreateMaterialResource()
{
    materialResource_ = DX12Helper::CreateBufferResource(device_, sizeof(Material));

    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

    materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_->enableLighting = false;
    materialData_->uvTransform = Matrix4x4::Identity();
}


/// TransformMatrix用のリソースを作成する
void Sprite::CreateTransformationMatrixResource()
{
    transformationMatrixResource_ = DX12Helper::CreateBufferResource(device_, sizeof(TransformationMatrix)); // TransformationMatrix
    // データを書き込む
    transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
    // 単位行列を書き込んでおく
    transformationMatrixData_->wvp = Matrix4x4::Identity();
    transformationMatrixData_->world = Matrix4x4::Identity();
}
