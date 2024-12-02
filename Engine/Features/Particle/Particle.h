#pragma once

#include <string>

#include <Core/DirectX12/DirectX12.h>
#include <Common/structs.h>
#include <wrl.h>
#include <d3d12.h>
#include <Features/Model/Model.h>
#include <thread>

class Particle
{
public:
    void Initialize(std::string _filepath);
    void Draw();
    void Upload();

private:
    static const uint32_t kInstanceCount_;

    Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_ = nullptr;
    std::string modelPath_ = {};
    DirectX12* pDx12_ = nullptr;
    ID3D12Device* pDevice_ = nullptr;
    TransformationMatrix* instancingData_ = nullptr;
    Model* pModel_ = nullptr;
    ModelData* pModelData_;
    uint32_t instanceCount_ = 10;

private:
    void CreateTransformationMatrixResource();
    void GetModelData();
};