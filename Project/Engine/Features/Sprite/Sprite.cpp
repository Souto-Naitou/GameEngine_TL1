#include "Sprite.h"

#include "SpriteSystem.h"
#include <Core/DirectX12/DX12Helper.h>
#include <Matrix4x4.h>
#include <Core/DirectX12/TextureManager.h>

Sprite::Sprite()
{

}


Sprite::~Sprite()
{
}


void Sprite::Initialize(SpriteSystem* _spriteSystem, std::string _filepath)
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

    TextureManager::GetInstance()->LoadTexture(texturePath);
    textureIndex_ = TextureManager::GetInstance()->GetTextureIndexByFilePath(texturePath);

    AdjustSpriteSize();
}


void Sprite::Update()
{
    uint32_t clientWidth = pDx12_->GetClientWidth();
    uint32_t clientHeight = pDx12_->GetClientHeight();
    const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(textureIndex_);


    // 左下
    vertexData_[0].normal = { 0.0f, 0.0f, -1.0f };
    // 左上
    vertexData_[1].normal = { 0.0f, 0.0f, -1.0f };
    // 右下
    vertexData_[2].normal = { 0.0f, 0.0f, -1.0f };
    // 右上
    vertexData_[3].normal = { 0.0f, 0.0f, -1.0f };

    /// アンカーポイントの設定と適用
    float left      = 0.0f - anchorPoint_.x;
    float right     = 1.0f - anchorPoint_.x;
    float top       = 0.0f - anchorPoint_.y;
    float bottom    = 1.0f - anchorPoint_.y;

    if (isFlipX)
    {
        left = -left;
        right = -right;
    }

    if (isFlipY)
    {
        top = -top;
        bottom = -bottom;
    }

    vertexData_[0].position = { left, bottom, 0.0f, 1.0f };
    vertexData_[1].position = { left, top, 0.0f, 1.0f };
    vertexData_[2].position = { right, bottom, 0.0f, 1.0f };
    vertexData_[3].position = { right, top, 0.0f, 1.0f };

    /// UV座標の設定と適用 (範囲指定)
    float tex_left = textureLeftTop_.x / metadata.width;
    float tex_top = textureLeftTop_.y / metadata.height;
    float tex_right = (textureLeftTop_.x + textureSize_.x) / metadata.width;
    float tex_bottom = (textureLeftTop_.y + textureSize_.y) / metadata.height;

    vertexData_[0].texcoord = { tex_left, tex_bottom };
    vertexData_[1].texcoord = { tex_left, tex_top };
    vertexData_[2].texcoord = { tex_right, tex_bottom };
    vertexData_[3].texcoord = { tex_right, tex_top };

    /// トランスフォームの更新
    transform_.scale = { size_.x, size_.y, 1.0f };
    transform_.rotate = { 0.0f, 0.0f, rotate_ };
    transform_.translate = translate_;

    /// WVPMatrixの更新
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
    commandList->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex_));

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

void Sprite::AdjustSpriteSize()
{
    // テクスチャのサイズを取得
    const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(textureIndex_);

    // テクスチャのサイズを取得
    float textureWidth = static_cast<float>(metadata.width);
    float textureHeight = static_cast<float>(metadata.height);

    // サイズを調整
    size_ = Vector2(textureWidth, textureHeight);
    textureLeftTop_ = Vector2(0.0f, 0.0f);
    textureSize_ = Vector2(textureWidth, textureHeight);
}
