#pragma once

#include "DirectX12.h"
#include "SRVManager.h"

#include <wrl/client.h>

class PostEffect
{
public:
    PostEffect() = default;
    ~PostEffect() = default;
    void Initialize();
    void NewFrame();
    void Draw();


public:
    ID3D12Resource* GetRenderTexture() const
    {
        return renderTexture_.Get();
    }


private:
    static constexpr wchar_t kVertexShaderPath[] = L"EngineResources/Shaders/CopyImage.VS.hlsl";
    static constexpr wchar_t kPixelShaderPath[] = L"EngineResources/Shaders/CopyImage.PS.hlsl";
    Microsoft::WRL::ComPtr<ID3D12Resource>      renderTexture_          = nullptr;
    uint32_t                                    renderSRVIndex_         = 0;
    uint32_t                                    rtvHeapIndex_           = 0;
    D3D12_CPU_DESCRIPTOR_HANDLE                 rtvHandle_              = {};
    D3D12_GPU_DESCRIPTOR_HANDLE                 rtvHandleGpu_           = {};
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_          = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_          = nullptr;
    D3D12_INPUT_LAYOUT_DESC                     inputLayoutDesc_        = {};
    D3D12_GRAPHICS_PIPELINE_STATE_DESC          pipelineStateDesc_      = {};
    D3D12_RASTERIZER_DESC                       rasterizerDesc_         = {};
    Microsoft::WRL::ComPtr<IDxcBlob>            vertexShaderBlob_       = nullptr;
    Microsoft::WRL::ComPtr<IDxcBlob>            pixelShaderBlob_        = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pso_                    = nullptr;
    


private:
    void CreateRenderTexture();
    void CreateSRV();
    void ObtainInstances();
    void CreateRootSignature();
    void CreatePipelineState();


private:
    DirectX12*                              pDx12_              = nullptr;
    RTVHeapCounter*                         rtvHeapCounter_     = nullptr;
    ID3D12Device*                           pDevice_            = nullptr;
    SRVManager*                             pSRVManager_        = nullptr;
    ID3D12GraphicsCommandList*              commandList_        = nullptr;
    D3D12_VIEWPORT                          viewport_           = {};
    D3D12_RECT                              scissorRect_        = {};
    ID3D12DescriptorHeap*                   dsvHeap_            = {};
    ID3D12DescriptorHeap*                   rtvHeap_            = {};
    Vector4                                 editorBG_           = {};
};