#include "Line.h"
#include "Line.h"

#include <Core/DirectX12/Helper/DX12Helper.h>

void Line::Initialize()
{
    /// 必要なインスタンスを取得
    pLineSystem_ = LineSystem::GetInstance();
    pDx12_ = pLineSystem_->GetDx12();
    device_ = pDx12_->GetDevice();

    pGameEye_ = pLineSystem_->GetDefaultGameEye();

    CreateVertexResource();
    CreateWVPMatrixResource();
    CreateColorResource();
}

void Line::Finalize()
{
    OutputDebugStringA("Line::Finalize\n");
}

void Line::Update()
{
    Matrix4x4 wMatrix = Matrix4x4::Identity();
    Matrix4x4 vpMatrix = pGameEye_->GetViewProjectionMatrix();

    pWVPMatrixData_[0] = wMatrix * vpMatrix;

    pVertexData_[0] = vertices_[0];
    pVertexData_[1] = vertices_[1];

    pColorData_[0] = color_;
}

void Line::Draw()
{
    ID3D12GraphicsCommandList* commandList = pDx12_->GetCommandList();

    commandList->SetGraphicsRootConstantBufferView(0, colorResource_->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(1, wvpMatrixResource_->GetGPUVirtualAddress());
    /// 頂点バッファをセットする
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    /// 描画
    commandList->DrawInstanced(2, 1, 0, 0);
}

void Line::CreateVertexResource()
{
    /// 頂点バッファリソースを作成
    vertexResource_ = DX12Helper::CreateBufferResource(device_, sizeof(Vector3) * 2);
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&pVertexData_));
    /// 頂点データを初期化
    pVertexData_[0] = Vector3(0.0f, 0.0f, 0.0f);
    pVertexData_[1] = Vector3(1.0f, 1.0f, 0.0f);

    /// 頂点バッファービューを初期化
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = static_cast<uint32_t>(sizeof(Vector3) * 2);
    vertexBufferView_.StrideInBytes = sizeof(Vector3);
}

void Line::CreateWVPMatrixResource()
{
    /// 座標変換行列リソースを作成
    wvpMatrixResource_ = DX12Helper::CreateBufferResource(device_, sizeof(Matrix4x4));
    wvpMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&pWVPMatrixData_));
    /// 座標変換行列データを初期化
    pWVPMatrixData_[0] = Matrix4x4::Identity();
}

void Line::CreateColorResource()
{
    /// 色リソースを作成
    colorResource_ = DX12Helper::CreateBufferResource(device_, sizeof(Vector4));
    colorResource_->Map(0, nullptr, reinterpret_cast<void**>(&pColorData_));
    /// 色データを初期化
    pColorData_[0] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}
