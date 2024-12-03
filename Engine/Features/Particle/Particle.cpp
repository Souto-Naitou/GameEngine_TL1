#include "Particle.h"
#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Features/Model/ModelManager.h>
#include <Features/Model/Helper/ModelHelper.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <Core/DirectX12/SRVManager.h>


void Particle::Initialize(ParticleSystem* _system, std::string _filepath)
{
    /// 必要なインスタンスを取得
    pDx12_ = DirectX12::GetInstance();
    pDevice_ = pDx12_->GetDevice();
    pSystem_ = _system;

    /// デフォルトのGameEyeを取得
    pGameEye_ = pSystem_->GetDefaultGameEye();

    /// 座標変換行列リソースを作成
    CreateParticleForGPUResource();

    /// SRVを作成
    CreateSRV();

    modelPath_ = _filepath;

    ModelManager::GetInstance()->LoadModel(_filepath);
    pModel_ = ModelManager::GetInstance()->FindModel(_filepath);

    if (pModel_->IsUploaded()) GetModelData();
}

void Particle::Draw()
{
    if (!pModel_->IsUploaded()) return;
    auto commandList = pDx12_->GetCommandList();

    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList->SetGraphicsRootDescriptorTable(0, srvGpuHandle_);
    commandList->SetGraphicsRootDescriptorTable(1, textureSRVHandleGPU_);
    commandList->DrawInstanced(static_cast<UINT>(pModelData_->vertices.size()), instanceCount_, 0, 0);
}

void Particle::Update()
{
    if (!pModel_) pModel_ = ModelManager::GetInstance()->FindModel(modelPath_);
    else if (pModel_->IsUploaded()) GetModelData();

    for (uint32_t index = 0; index < kInstanceCount_; ++index)
    {
        transforms_[index].scale = Vector3(-1.0f, 1.0f, 1.0f);
        transforms_[index].rotate = Vector3(0.0f, 0.0f, 0.0f);
        transforms_[index].translate = Vector3(0.0f, 0.0f, index);
        Matrix4x4 wMatrix = Matrix4x4::AffineMatrix(transforms_[index].scale, transforms_[index].rotate, transforms_[index].translate);
        instancingData_[index].world = wMatrix;
        instancingData_[index].wvp = wMatrix * pGameEye_->GetViewProjectionMatrix();
        instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void Particle::CreateParticleForGPUResource()
{
    /// 座標変換行列リソースを作成
    instancingResource_ = DX12Helper::CreateBufferResource(pDevice_, sizeof(ParticleForGPU) * kInstanceCount_);
    instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
    /// 座標変換行列データを初期化
    for (uint32_t index = 0; index < kInstanceCount_; ++index)
    {
        instancingData_[index].wvp = Matrix4x4::Identity();
        instancingData_[index].world = Matrix4x4::Identity();
        instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void Particle::CreateVertexBuffer()
{
}

void Particle::CreateSRV()
{
    SRVManager* srvManager = SRVManager::GetInstance();
    srvIndex_ = srvManager->Allocate();
    srvCpuHandle_ = srvManager->GetCPUDescriptorHandle(srvIndex_);
    srvGpuHandle_ = srvManager->GetGPUDescriptorHandle(srvIndex_);

    srvManager->CreateForStructuredBuffer(srvIndex_, instancingResource_.Get(), kInstanceCount_, sizeof(ParticleForGPU));
    return;
}

void Particle::GetModelData()
{
    pModelData_ = pModel_->GetModelData();
    vertexBufferView_ = pModel_->GetVertexBufferView();
    textureSRVHandleGPU_ = pModel_->GetTextureSrvHandleGPU();
}
