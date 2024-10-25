#include "Core/DirectX12/DirectX12.h"

#include "Logger.h"
#include "ConvertString.h"
#include <cassert>
#include <format>
#include "DX12Helper.h"

/// DirectX12クラスの実装部分

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


    /// ディスクリプタヒープの生成も行う
    rtvDescriptorHeap_ = DX12Helper::CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
    srvDescriptorHeap_ = DX12Helper::CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);


    /// SwapChainからResourceを引っ張ってくる
    hr_ = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
    assert(SUCCEEDED(hr_) && "SwapChainからリソースを取得できませんでした [0]");
    hr_ = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
    assert(SUCCEEDED(hr_) && "SwapChainからリソースを取得できませんでした [1]");


    /// RTVの設定
    rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
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
