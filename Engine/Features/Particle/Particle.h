#pragma once

#include <string>

#include <Core/DirectX12/DirectX12.h>
#include <Common/structs.h>
#include <wrl.h>
#include <d3d12.h>
#include <Features/Model/Model.h>
#include <thread>
#include "ParticleSystem.h"

class Particle
{
public:
    void Initialize(ParticleSystem* _system, std::string _filepath);
    void Draw();
    void Update();

private:
    static constexpr uint32_t               kInstanceCount_ = 10u;

    /// Transform
    Transform                               transforms_[kInstanceCount_]        = {};

    /// GameEye
    GameEye*                                pGameEye_                           = nullptr;

    /// Instancing
    Microsoft::WRL::ComPtr<ID3D12Resource>  instancingResource_                 = nullptr;
    ParticleForGPU*                         instancingData_                     = nullptr;
    uint32_t                                instanceCount_                      = 10u;

    /// Model
    std::string                             modelPath_                          = {};
    Model*                                  pModel_                             = nullptr;
    ModelData*                              pModelData_;
    D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_                   = {};

    /// SRV
    uint32_t                                srvIndex_                           = 0u;
    D3D12_CPU_DESCRIPTOR_HANDLE             srvCpuHandle_                       = {};
    D3D12_GPU_DESCRIPTOR_HANDLE             srvGpuHandle_                       = {};
    D3D12_GPU_DESCRIPTOR_HANDLE             textureSRVHandleGPU_                = {};


private: /// 他クラスのインスタンス
    DirectX12* pDx12_ = nullptr;
    ID3D12Device* pDevice_ = nullptr;
    ParticleSystem* pSystem_ = nullptr;


private:
    void CreateParticleForGPUResource();
    void CreateVertexBuffer();
    void CreateSRV();
    void GetModelData();
};