#pragma once

#include <wrl.h>
#include <stdint.h>
#include <string>
#include <memory>
#include "Framerate.h"
#include <d3d12.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <d3d11on12.h>

/// DirectWrite
#include <d2d1_3.h>
#include <dwrite_3.h>


#include <vector>
#include <array>
#include <DebugTools/ReakChecker.h>

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
    ID3D12Device*                                           GetDevice() const                           { return device_.Get(); }
    ID3D12GraphicsCommandList*                              GetCommandList() const                      { return commandList_.Get(); }
    IDxcUtils*                                              GetDxcUtils() const                         { return dxcUtils_.Get(); }
    IDxcCompiler3*                                          GetDxcCompiler() const                      { return dxcCompiler_.Get(); }
    IDxcIncludeHandler*                                     GetIncludeHandler() const                   { return includeHandler_.Get(); }

    uint32_t                                                GetClientWidth() const                      { return clientWidth_; }
    uint32_t                                                GetClientHeight() const                     { return clientHeight_; }

    uint32_t                                                GetDescriptorSizeSRV() const                { return kDescriptorSizeSRV; }

    int32_t                                                 GetNumUploadedTexture() const               { return numUploadedTexture; }

    const DXGI_SWAP_CHAIN_DESC1&                            GetSwapChainDesc() const                    { return swapChainDesc_; }

    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>&               GetSRVHandlesGPUList()                      { return srvHandlesGPUList_; }
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>&               GetSRVHandlesCPUList()                      { return srvHandlesCPUList_; }
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>&    GetTextureResources()                       { return textureResources_; }
    IDXGIDevice*                                            GetDXGIDevice()                             { return dxgiDevice_.Get(); }
    ID2D1Factory3*                                          GetD2D1Factory()                            { return d2dFactory_.Get(); }
    ID2D1Device2*                                           GetDirect2dDevice()                         { return d2dDevice_.Get(); }
    ID2D1DeviceContext2*                                    GetDirect2dDeviceContext()                  { return d2dDeviceContext_.Get(); }
    uint32_t&                                               GetGameWidthRef()                           { return gameWindowWidth_; }
    uint32_t&                                               GetGameHeightRef()                          { return gameWindowHeight_; }

private:
    DirectX12() = default;
    ~DirectX12();

    HRESULT                                                 hr_                             = 0;
    HWND                                                    hwnd_                           = {};

    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>                srvHandlesGPUList_              = {};           // SRVハンドルリスト(GPU)
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>                srvHandlesCPUList_              = {};           // SRVハンドルリスト(CPU)
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>     textureResources_               = {};           // テクスチャリソース
    Microsoft::WRL::ComPtr<ID3D12Device>                    device_                         = nullptr;      // デバイス
    Microsoft::WRL::ComPtr<ID3D12Debug1>                    debugController_                = nullptr;      // デバッグコントローラ
    Microsoft::WRL::ComPtr<IDXGIFactory7>                   dxgiFactory_                    = nullptr;      // DXGIファクトリ
    Microsoft::WRL::ComPtr<IDXGIAdapter4>                   useAdapter_                     = nullptr;      // 使うアダプタ
    Microsoft::WRL::ComPtr<ID3D12InfoQueue>                 infoQueue_                      = nullptr;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>              commandQueue_                   = nullptr;      // コマンドキュー
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>          commandAllocator_               = nullptr;      // コマンドアロケータ
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>       commandList_                    = nullptr;      // コマンドアロケータ
    Microsoft::WRL::ComPtr<IDXGISwapChain4>                 swapChain_                      = nullptr;      // スワップチェーン
    Microsoft::WRL::ComPtr<ID3D12Resource>                  swapChainResources_[2]          = {};           // スワップチェーンリソース
    Microsoft::WRL::ComPtr<ID3D12Fence>                     fence_                          = nullptr;      // フェンス
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>            rtvDescriptorHeap_              = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>            dsvDescriptorHeap_              = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>            descriptorHeaps_                = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>                  depthStencilResource_           = nullptr;      // 深度ステンシルリソース
    Microsoft::WRL::ComPtr<IDxcUtils>                       dxcUtils_                       = nullptr;
    Microsoft::WRL::ComPtr<IDxcCompiler3>                   dxcCompiler_                    = nullptr;
    Microsoft::WRL::ComPtr<IDxcIncludeHandler>              includeHandler_                 = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Device>                    d3d11Device_                    = nullptr;      // D3D11デバイス
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>             d3d11DeviceContext_             = nullptr;      // D3D11デバイスコンテキスト
    Microsoft::WRL::ComPtr<ID3D11On12Device>                d3d11On12Device_                = nullptr;      // D3D11On12デバイス
    Microsoft::WRL::ComPtr<ID2D1Factory3>                   d2dFactory_                     = nullptr;      // Direct2D
    Microsoft::WRL::ComPtr<IDXGIDevice>                     dxgiDevice_                     = nullptr;      // DXGIデバイス
    Microsoft::WRL::ComPtr<ID2D1Device2>                    d2dDevice_                      = nullptr;      // d2d1デバイス
    Microsoft::WRL::ComPtr<ID2D1DeviceContext2>             d2dDeviceContext_               = nullptr;      // d2d1デバイスコンテキスト

    std::array<Microsoft::WRL::ComPtr<ID3D11Resource>, 2>   d3d11WrappedBackBuffers_        = {};           // D3D11のラップされたバックバッファ
    std::array<Microsoft::WRL::ComPtr<ID2D1Bitmap1>, 2>     d2dRenderTargets_               = {};           // D2D1のレンダーターゲット

    D3D12_RESOURCE_BARRIER                                  barrier_                        = {};
    D3D12_COMMAND_QUEUE_DESC                                commandQueueDesc_               = {};           // コマンドキューの設定
    DXGI_SWAP_CHAIN_DESC1                                   swapChainDesc_                  = {};           // スワップチェーンの設定
    D3D12_RENDER_TARGET_VIEW_DESC                           rtvDesc_                        = {};           // RTVの設定
    HANDLE                                                  fenceEvent_                     = {};
    uint64_t                                                fenceValue_                     = 0u;           // フェンス値
    D3D12_VIEWPORT                                          viewport_                       = {};           // ビューポート
    D3D12_RECT                                              scissorRect_                    = {};           // シザーレクト
    D3D12_CPU_DESCRIPTOR_HANDLE                             rtvHandles_[2]                  = {};           // RTVハンドル
    uint32_t                                                clientWidth_                    = 1280u;
    uint32_t                                                clientHeight_                   = 720u;
    uint32_t                                                gameWindowWidth_                = 1280u;
    uint32_t                                                gameWindowHeight_               = 720u;
    float                                                   clearColor_[4]                  = { 0.2f, 0.2f, 0.4f, 1.0f };
    uint32_t                                                backBufferIndex_                = 0u;
    uint32_t                                                kDescriptorSizeSRV              = 0u;
    uint32_t                                                kDescriptorSizeRTV              = 0u;
    uint32_t                                                kDescriptorSizeDSV              = 0u;
    int32_t                                                 numUploadedTexture              = 0;

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
    /// Direct2Dのファクトリの生成
    /// </summary>
    void CreateD2D1Factory();


    /// <summary>
    /// D3D11デバイスの生成
    /// </summary>
    void CreateD3D11Device();


    /// <summary>
    /// デバイスコンテキストの生成
    /// </summary>
    void CreateID2D1DeviceContext();


    /// <summary>
    /// テクスチャのアップロード
    /// </summary>
    void CreateD2DRenderTarget();


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