#pragma once

#include <string>

#include <Core/DirectX12/DirectX12.h>
#include <Common/structs.h>
#include <wrl.h>
#include <d3d12.h>
#include <Features/Model/Model.h>
#include "ParticleSystem.h"
#include <Features/IContainer/VectorContainer.h>
#include <Matrix4x4.h>

class Particle : public VectorContainer<Transform>
{
public:
    void Initialize(ParticleSystem* _system, std::string _filepath);
    void Draw();
    void Update();
    void Finalize();


public: /// Setter
    void SetName(const std::string& _name) { name_ = _name; }
    void SetEnableBillboard(bool _enable) { enableBillboard_ = _enable; }


public: /// Getter
    const std::string& GetName() const { return name_; }
    bool GetEnableBillboard() const { return enableBillboard_; }


public: /// Override
    void reserve(size_t _size) override;


private:
    /// Common
    std::string                             name_                               = {};

    /// GameEye
    GameEye*                                pGameEye_                           = nullptr;

    /// Instancing
    Microsoft::WRL::ComPtr<ID3D12Resource>  instancingResource_                 = nullptr;
    ParticleForGPU*                         instancingData_                     = nullptr;

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

    /// Billboard
    Matrix4x4                               backToFrontMatrix_                  = {};
    Matrix4x4                               billboardMatrix_                    = {};
    bool                                    enableBillboard_                    = false;


private: /// 他クラスのインスタンス
    DirectX12* pDx12_ = nullptr;
    ID3D12Device* pDevice_ = nullptr;
    ParticleSystem* pSystem_ = nullptr;


private:
    void CreateParticleForGPUResource();
    void CreateSRV();
    void GetModelData();
    void InitializeTransform();
    void DebugWindow();
};