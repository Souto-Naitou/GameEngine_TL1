#pragma once

#include <wrl.h>
#include <Windows.h>
#include <stdint.h>
#include <string>
#include <memory>
#include "Framerate.h"

class DirectX12
{
public:
    DirectX12(const DirectX12&) = delete;
    DirectX12(const DirectX12&&) = delete;
    DirectX12& operator=(const DirectX12&) = delete;
    DirectX12& operator=(const DirectX12&&) = delete;

    static DirectX12* GetInstance()
    {
        static D3DResourceLeakChecker leakchecker;
        static DirectX12 instance; return &instance;
    }

    void Initialize();

    void PresentDraw();
    void PostDraw();


    static const uint32_t kMaxSRVCount_;

public: /// Getter
    ID3D12Device*                               GetDevice()                         const   { return device_.Get(); }
    ID3D12GraphicsCommandList*                  GetCommandList()                    const   { return commandList_.Get(); }
    IDxcUtils*                                  GetDxcUtils()                       const   { return dxcUtils_.Get(); }
    IDxcCompiler3*                              GetDxcCompiler()                    const   { return dxcCompiler_.Get(); }
    IDxcIncludeHandler*                         GetIncludeHandler()                 const   { return includeHandler_.Get(); }

    uint32_t                                    GetClientWidth()                    const   { return clientWidth_; }
    uint32_t                                    GetClientHeight()                   const   { return clientHeight_; }

    uint32_t                                    GetDescriptorSizeSRV()              const   { return kDescriptorSizeSRV; }

    int32_t                                     GetNumUploadedTexture()             const   { return numUploadedTexture; }

    const DXGI_SWAP_CHAIN_DESC1&                GetSwapChainDesc()                  const   { return swapChainDesc_; }

    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>&   GetSRVHandlesGPUList()                      { return srvHandlesGPUList_; }
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>&   GetSRVHandlesCPUList()                      { return srvHandlesCPUList_; }
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& GetTextureResources()              { return textureResources_; }

private:
    DirectX12() = default;
    ~DirectX12();

    HRESULT hr_     = 0;
    HWND    hwnd_   = {};


    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>            srvHandlesGPUList_              = {};           // SRVハンドルリスト(GPU)
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>            srvHandlesCPUList_              = {};           // SRVハンドルリスト(CPU)
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> textureResources_               = {};           // テクスチャリソース

    Microsoft::WRL::ComPtr<ID3D12Device>                device_                         = nullptr;      // デバイス
    Microsoft::WRL::ComPtr<ID3D12Debug1>                debugController_                = nullptr;      // デバッグコントローラ
    Microsoft::WRL::ComPtr<IDXGIFactory7>               dxgiFactory_                    = nullptr;      // DXGIファクトリ
    Microsoft::WRL::ComPtr<IDXGIAdapter4>               useAdapter_                     = nullptr;      // 使うアダプタ
    Microsoft::WRL::ComPtr<ID3D12InfoQueue>             infoQueue_                      = nullptr;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>          commandQueue_                   = nullptr;      // コマンドキュー
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      commandAllocator_               = nullptr;      // コマンドアロケータ
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   commandList_                    = nullptr;      // コマンドアロケータ
    Microsoft::WRL::ComPtr<IDXGISwapChain4>             swapChain_                      = nullptr;      // スワップチェーン
    Microsoft::WRL::ComPtr<ID3D12Resource>              swapChainResources_[2]          = {};           // スワップチェーンリソース
    Microsoft::WRL::ComPtr<ID3D12Fence>                 fence_                          = nullptr;      // フェンス
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        rtvDescriptorHeap_              = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        dsvDescriptorHeap_              = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        descriptorHeaps_                = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>              depthStencilResource_           = nullptr;      // 深度ステンシルリソース
    Microsoft::WRL::ComPtr<IDxcUtils>                   dxcUtils_                       = nullptr;
    Microsoft::WRL::ComPtr<IDxcCompiler3>               dxcCompiler_                    = nullptr;
    Microsoft::WRL::ComPtr<IDxcIncludeHandler>          includeHandler_                 = nullptr;
    D3D12_RESOURCE_BARRIER                              barrier_                        = {};
    D3D12_COMMAND_QUEUE_DESC                            commandQueueDesc_               = {};           // コマンドキューの設定
    DXGI_SWAP_CHAIN_DESC1                               swapChainDesc_                  = {};           // スワップチェーンの設定
    D3D12_RENDER_TARGET_VIEW_DESC                       rtvDesc_                        = {};           // RTVの設定
    HANDLE                                              fenceEvent_                     = {};
    uint64_t                                            fenceValue_                     = 0u;           // フェンス値
    D3D12_VIEWPORT                                      viewport_                       = {};           // ビューポート
    D3D12_RECT                                          scissorRect_                    = {};           // シザーレクト

    D3D12_CPU_DESCRIPTOR_HANDLE                         rtvHandles_[2]                  = {};           // RTVハンドル

    uint32_t                                            clientWidth_                    = 1280u;
    uint32_t                                            clientHeight_                   = 720u;

    float                                               clearColor_[4]                  = { 0.2f, 0.2f, 0.4f, 1.0f };

    uint32_t                                            backBufferIndex_                = 0u;

    uint32_t                                            kDescriptorSizeSRV              = 0u;
    uint32_t                                            kDescriptorSizeRTV              = 0u;
    uint32_t                                            kDescriptorSizeDSV              = 0u;

    int32_t                                             numUploadedTexture              = 0;

private:

    /// <summary>
    /// アダプタの選択
    /// </summary>
    void ChooseAdapter();


    /// <summary>
    /// Command[List/Queue/Allocator]を生成する
    /// </summary>
    void CreateCommandResources();


    /// <summary>
    /// スワップチェーンの生成
    /// </summary>
    void CreateSwapChainAndResource();


    /// <summary>
    /// DepthStencilViewの生成
    /// </summary>
    void CreateDSVAndSettingState();


    /// <summary>
    /// フェンスの生成とイベントの生成
    /// </summary>
    void CreateFenceAndEvent();


    /// <summary>
    /// ビューポートとシザー矩形の設定
    /// </summary>
    void SetViewportAndScissorRect();


    /// <summary>
    /// DirectXShaderCompilerの生成
    /// </summary>
    void CreateDirectXShaderCompiler();


    /// <summary>
    /// リソースバリアの設定
    /// </summary>
    /// <param name="_type">バリアの種類</param>
    /// <param name="_flag">フラグ</param>
    /// <param name="_resource">リソース</param>
    /// <param name="_before">変更前のステート</param>
    /// <param name="_after">変更後のステート</param>
    void SetResourceBarrier(
        D3D12_RESOURCE_BARRIER_TYPE _type,
        D3D12_RESOURCE_BARRIER_FLAGS _flag,
        ID3D12Resource* _resource,
        D3D12_RESOURCE_STATES _before,
        D3D12_RESOURCE_STATES _after
    );

private: /// 他クラスのインスタンス(シングルトンなど)
    FrameRate* pFramerate_ = nullptr;

};