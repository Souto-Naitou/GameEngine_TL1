#include "Particle.h"
#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Features/Model/ModelManager.h>
#include <Features/Model/Helper/ModelHelper.h>
#include <DebugTools/DebugManager/DebugManager.h>

const uint32_t Particle::kInstanceCount_ = 256u;

void Particle::Initialize(std::string _filepath)
{
    /// 必要なインスタンスを取得
    pDx12_ = DirectX12::GetInstance();
    pDevice_ = pDx12_->GetDevice();

    CreateTransformationMatrixResource();

    modelPath_ = _filepath;

    ModelManager::GetInstance()->LoadModel(_filepath);
    pModel_ = ModelManager::GetInstance()->FindModel(_filepath);

    if (pModel_) GetModelData();
}

void Particle::Draw()
{
    if (!pModelData_) return;
    auto commandList = pDx12_->GetCommandList();
    commandList->DrawInstanced(pModelData_->vertices.size(), instanceCount_, 0, 0);
}

void Particle::Upload()
{
    if (!pModel_) pModel_ = ModelManager::GetInstance()->FindModel(modelPath_);
    else if (pModel_ && !pModelData_) GetModelData();
}

void Particle::CreateTransformationMatrixResource()
{
    /// 座標変換行列リソースを作成
    instancingResource_ = DX12Helper::CreateBufferResource(pDevice_, sizeof(TransformationMatrix) * kInstanceCount_);
    instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
    /// 座標変換行列データを初期化
    for (uint32_t index = 0; index < kInstanceCount_; ++index)
    {
        instancingData_[index].wvp = Matrix4x4::Identity();
        instancingData_[index].world = Matrix4x4::Identity();
    }
}

void Particle::GetModelData()
{
    pModelData_ = pModel_->GetModelData();
}
