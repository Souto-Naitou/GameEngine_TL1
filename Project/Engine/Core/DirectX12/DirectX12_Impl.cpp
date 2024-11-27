/// DirectX12クラスの実装部分

#include "DirectX12.h"

#include <Utility/Logger.h>
#include <Utility/ConvertString.h>
#include <cassert>
#include <format>
#include "DX12Helper.h"

/// ImGui
#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#endif // _DEBUG

#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

const uint32_t DirectX12::kMaxSRVCount_ = 512ui32;

void DirectX12::ChooseAdapter()
{
    for (uint32_t i = 0;
        dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND;
        i++)
    {
        /// アダプタ情報を取得
        DXGI_ADAPTER_DESC3 adapterDesc{};
        hr_ = useAdapter_->GetDesc3(&adapterDesc);
        assert(SUCCEEDED(hr_) && "アダプター情報の取得に失敗");
        // ソフトウェアアダプタではなければ採用
        if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
        {
            // 出力
            Log(std::format("Adapter : {}\n", ConvertString(adapterDesc.Description)));
            break;
        }
        useAdapter_ = nullptr;
    }
    assert(useAdapter_ != nullptr && "アダプタが見つかりませんでした。");
}

void DirectX12::CreateCommandResources()
{
    /// コマンドキューを生成
    hr_ = device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
    assert(SUCCEEDED(hr_) && "コマンドキューの生成に失敗");
    if (!commandQueue_) return;


    /// コマンドアロケータを生成
    hr_ = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
    assert(SUCCEEDED(hr_) && "コマンドアロケータの生成に失敗");
    if (!commandAllocator_) return;

    /// コマンドリストを生成
    hr_ = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
    assert(SUCCEEDED(hr_) && "コマンドリストの生成がうまくいかなかったので起動できない");

}

void DirectX12::CreateSwapChainAndResource()
{
    /// スワップチェーンの設定
    swapChainDesc_ = {};
    swapChainDesc_.Width                = clientWidth_;                         // 画面の幅。ウィンドウのクライアント領域を同じものにしておく
    swapChainDesc_.Height               = clientHeight_;                        // 画面の高さ。ウィンドウのクライアント領域を同じものにしておく
    swapChainDesc_.Format               = DXGI_FORMAT_R8G8B8A8_UNORM;           // 色の形式
    swapChainDesc_.SampleDesc.Count     = 1;                                    // マルチサンプルしない
    swapChainDesc_.BufferUsage          = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // 描画のターゲットとして利用する
    swapChainDesc_.BufferCount          = 2;                                    // ダブルバッファ
    swapChainDesc_.SwapEffect           = DXGI_SWAP_EFFECT_FLIP_DISCARD;        // モニタにうつしたら、中身を破棄


    /// 生成
    hr_ = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), hwnd_, &swapChainDesc_, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
    assert(SUCCEEDED(hr_));


    /// Descriptorのサイズを取得 (動的に変わらないもの)
    kDescriptorSizeSRV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    kDescriptorSizeRTV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    kDescriptorSizeDSV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


    /// ディスクリプタヒープの生成も行う
    rtvDescriptorHeap_ = DX12Helper::CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
    srvDescriptorHeap_ = DX12Helper::CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount_, true);
    dsvDescriptorHeap_ = DX12Helper::CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);


    /// SwapChainからResourceを引っ張ってくる
    hr_ = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
    assert(SUCCEEDED(hr_) && "SwapChainからリソースを取得できませんでした [0]");
    hr_ = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
    assert(SUCCEEDED(hr_) && "SwapChainからリソースを取得できませんでした [1]");
    swapChain_->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709);


    /// RTVの設定
    rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;


    /// RTVの生成
    D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
    // 1つ目のRTVを作成
    rtvHandles_[0] = rtvStartHandle;
    device_->CreateRenderTargetView(swapChainResources_[0].Get(), &rtvDesc_, rtvHandles_[0]);
    // 2つ目のRTVを作成
    rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    device_->CreateRenderTargetView(swapChainResources_[1].Get(), &rtvDesc_, rtvHandles_[1]);
}

void DirectX12::CreateDSVAndSettingState()
{
    /// リソースの生成
    depthStencilResource_ = DX12Helper::CreateDepthStencilTextureResource(device_, clientWidth_, clientHeight_);


    /// DSVの生成
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2DTexture
    device_->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc, dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());


    /// DepthStencilStateの設定
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = true;                            // 深度テストを有効化
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;   // 深度書き込み
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;  // 深度テストの比較条件式（近ければ描画）
    depthStencilDesc.StencilEnable = true;                          // ステンシルテストを有効
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;   // ステンシル書き込みを有効
}

void DirectX12::CreateFenceAndEvent()
{
    /// 初期値0でFenceを作る
    hr_ = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    assert(SUCCEEDED(hr_) && "フェンス生成に失敗");


    /// FenceのSignalを待つためのイベントを作成する
    fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
    assert(fenceEvent_ != nullptr);
}

void DirectX12::SetViewportAndScissorRect()
{
    /// ビューポート
    // クライアント領域のサイズと一緒にして画面全体に表示
    viewport_.Width = static_cast<FLOAT>(clientWidth_);
    viewport_.Height = static_cast<FLOAT>(clientHeight_);
    viewport_.TopLeftX = 0;
    viewport_.TopLeftY = 0;
    viewport_.MinDepth = 0.0f;
    viewport_.MaxDepth = 1.0f;


    /// シザー矩形
    // 基本的にビューポートと同じ矩形が構成されるようにする
    scissorRect_.left = 0;
    scissorRect_.right = clientWidth_;
    scissorRect_.top = 0;
    scissorRect_.bottom = clientHeight_;
}

void DirectX12::CreateDirectXShaderCompiler()
{
    /// dxcCompilerを初期化
    hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
    assert(SUCCEEDED(hr_));
    hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
    assert(SUCCEEDED(hr_));


    /// includeに対応するため
    hr_ = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
    assert(SUCCEEDED(hr_));
}

void DirectX12::SetResourceBarrier(
    D3D12_RESOURCE_BARRIER_TYPE _type, D3D12_RESOURCE_BARRIER_FLAGS _flag,
    ID3D12Resource* _resource,
    D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _after)
{
    /// ResourceBarrierの設定
    barrier_.Type = _type;                      // Transitionに設定
    barrier_.Flags = _flag;                     // フラグ
    barrier_.Transition.pResource = _resource;  // リソース
    barrier_.Transition.StateBefore = _before;  // 遷移前の状態
    barrier_.Transition.StateAfter = _after;    // 遷移後の状態


    /// Barrierを張る
    commandList_->ResourceBarrier(1, &barrier_);
}
