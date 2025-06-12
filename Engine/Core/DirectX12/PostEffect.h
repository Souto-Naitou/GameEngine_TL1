#pragma once

#include "DirectX12.h"
#include "SRVManager.h"

#include <wrl/client.h>
#include "ResourceStateTracker/ResourceStateTracker.h"
#include "IPostEffect.h"
#include <vector>

/// <ポストエフェクトを実行するクラス>
/// - 複数のポストエフェクトを順に適用するためにレンダーテクスチャのチェインを生成する
/// - Bloom -> MotionBlur の順で実行する場合 MotionBlurにBloomのレンダーテクスチャを渡す

class PostEffect
{
public:
    PostEffect(const PostEffect&) = delete;
    PostEffect& operator=(const PostEffect&) = delete;
    PostEffect(PostEffect&&) = delete;
    PostEffect& operator=(PostEffect&&) = delete;

    static PostEffect* GetInstance()
    {
        static PostEffect instance;
        return &instance;
    }

    void Initialize();
    void ApplyPostEffects();
    void NewFrame();
    void PostDraw();
    void Draw();
    void OnResize();
    void OnResizedBuffers();


public:
    PostEffect& AddPostEffect(IPostEffect* postEffect)
    {
        postEffects_.emplace_back(postEffect);
        return *this;
    }

    ID3D12Resource* GetRenderTexture() const
    {
        return renderTexture_.resource.Get();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE* GetRTVHandle()
    {
        return &rtvHandle_;
    }

    ID3D12GraphicsCommandList* GetCommandList()
    {
        return commandListForDraw_.Get();
    }

    D3D12_GPU_DESCRIPTOR_HANDLE GetRTVHandleGpu() const
    {
        return rtvHandleGpu_;
    }


private:
    PostEffect() = default;
    ~PostEffect() = default;

    static constexpr wchar_t kVertexShaderPath[] = L"EngineResources/Shaders/Fullscreen.VS.hlsl";
    static constexpr wchar_t kPixelShaderPath[] = L"EngineResources/Shaders/Fullscreen.PS.hlsl";
    ResourceStateTracker                                renderTexture_          = {};
    uint32_t                                            srvHeapIndex_           = 0;
    uint32_t                                            rtvHeapIndex_           = 0;
    D3D12_CPU_DESCRIPTOR_HANDLE                         rtvHandle_              = {};
    D3D12_GPU_DESCRIPTOR_HANDLE                         rtvHandleGpu_           = {};
    D3D12_GPU_DESCRIPTOR_HANDLE                         outputHandleGpu_        = {};
    D3D12_INPUT_LAYOUT_DESC                             inputLayoutDesc_        = {};
    D3D12_GRAPHICS_PIPELINE_STATE_DESC                  pipelineStateDesc_      = {};
    D3D12_RASTERIZER_DESC                               rasterizerDesc_         = {};
    Microsoft::WRL::ComPtr<ID3D12RootSignature>         rootSignature_          = nullptr;
    Microsoft::WRL::ComPtr<IDxcBlob>                    vertexShaderBlob_       = nullptr;
    Microsoft::WRL::ComPtr<IDxcBlob>                    pixelShaderBlob_        = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState>         pso_                    = nullptr;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   commandListForDraw_     = nullptr;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      commandAllocator_       = nullptr;

    std::vector<IPostEffect*>                           postEffects_           = {};


private:
    void ObtainInstances();
    void CreateRootSignature();
    void CreatePipelineState();
    void CreateCommandList();


private:
    DirectX12*                              pDx12_              = nullptr;
    RTVHeapCounter*                         rtvHeapCounter_     = nullptr;
    ID3D12Device*                           pDevice_            = nullptr;
    SRVManager*                             pSRVManager_        = nullptr;
    ID3D12GraphicsCommandList*              commandListMain_    = nullptr;
    D3D12_VIEWPORT                          viewport_           = {};
    D3D12_RECT                              scissorRect_        = {};
    ID3D12DescriptorHeap*                   dsvHeap_            = {};
    ID3D12DescriptorHeap*                   rtvHeap_            = {};
    D3D12_CPU_DESCRIPTOR_HANDLE             rtvHandleSwapChain_ = {};
    Vector4                                 editorBG_           = {};
};