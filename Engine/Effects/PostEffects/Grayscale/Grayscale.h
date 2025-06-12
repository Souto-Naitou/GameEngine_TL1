#pragma once

#include <Core/DirectX12/IPostEffect.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <dxcapi.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/ResourceStateTracker/ResourceStateTracker.h>

/// <グレースケール>
/// - PEはPost Effectの略
/// - ApplyメソッドとSettingメソッドはPostEffectクラスで実行する
class PEGrayscale : public IPostEffect
{
public:
    void    Initialize() override;
    void    Release() override;

private:
    // PostEffectクラスがアクセスする
    void    Apply() override;
    void    Setting() override;
    void    OnResizeBefore() override;
    void    OnResizedBuffers() override;
    void    ToShaderResourceState() override;

    // Setters
    void    Enable(bool _flag) override;
    void    SetInputTextureHandle(D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandle) override;
    // Getters
    bool    Enabled() const override;
    D3D12_GPU_DESCRIPTOR_HANDLE GetOutputTextureHandle() const override;
    const std::string&          GetName() const override;

private:
    ID3D12Device*                                       device_                 = nullptr;
    ID3D12GraphicsCommandList*                          commandList_            = nullptr;
    DirectX12*                                          pDx12_                  = nullptr;

    bool                                                isEnabled_              = true;
    const std::string                                   name_                   = "Grayscale";
    ResourceStateTracker                                renderTexture_          = {};
    Microsoft::WRL::ComPtr<IDxcBlob>                    vertexShaderBlob_       = nullptr;
    Microsoft::WRL::ComPtr<IDxcBlob>                    pixelShaderBlob_        = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState>         pso_                    = nullptr;
    Microsoft::WRL::ComPtr<ID3D12RootSignature>         rootSignature_          = nullptr;

    D3D12_CPU_DESCRIPTOR_HANDLE                         rtvHandleCpu_           = {};
    D3D12_GPU_DESCRIPTOR_HANDLE                         rtvHandleGpu_           = {};

    D3D12_GPU_DESCRIPTOR_HANDLE                         inputGpuHandle_         = {};

    uint32_t                                            rtvHeapIndex_           = 0;
    uint32_t                                            srvHeapIndex_           = 0;
    const std::wstring                                  kVertexShaderPath       = L"EngineResources/Shaders/Grayscale.VS.hlsl";
    const std::wstring                                  kPixelShaderPath        = L"EngineResources/Shaders/Grayscale.PS.hlsl";

    void    CreateRootSignature();
    void    CreatePipelineStateObject();
    void    ToRenderTargetState();
};