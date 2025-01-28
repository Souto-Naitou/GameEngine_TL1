#include "DirectX12.h"

#include <DebugTools/Logger/Logger.h>                   // ログ出力ラッパークラス
#include <Utility/ConvertString/ConvertString.h>        // wideとの互換
#include <Core/Win32/WinSystem.h>                       // Window関連
#include <Core/DirectX12/Helper/DX12Helper.h>           // ヘルパー
#include <Core/DirectX12/SRVManager.h>                  // SRV管理

#include <cassert>
#include <format>
#include <iostream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


/// <summary>
/// 初期化呼出
/// </summary>


void DirectX12::Initialize()
{
    /// デバッグコントローラの設定
#ifdef _DEBUG
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_))))
    {
        // デバッグレイヤーを有効化する
        debugController_->EnableDebugLayer();
        // GPU側もチェックする
        debugController_->SetEnableGPUBasedValidation(TRUE);
    }
#endif // _DEBUG

    pSRVManager_ = SRVManager::GetInstance();

    pFramerate_ = FrameRate::GetInstance();
    pFramerate_->Initialize();

    // ウィンドウハンドルを取得
    hwnd_ = WinSystem::GetInstance()->GetHwnd();

    // ウィンドウのクライアントサイズを取得
    clientWidth_ = WinSystem::kClientWidth;
    clientHeight_ = WinSystem::kClientHeight;

    hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
    assert(SUCCEEDED(hr_) && "DXGIファクトリの生成に失敗");


    /// アダプタ
    ChooseAdapter();


    /// デバイス生成
    DX12Helper::CreateDevice(&device_, useAdapter_);


    /// エラー時停止処理
    DX12Helper::PauseError(device_, infoQueue_);


    /// 出力ウィンドウに初期化完了を出力
    Log(std::format("DirectX12 Initialized.\n"));


    /// コマンド系を生成
    CreateCommandResources();


    /// スワップチェーンの生成とRTVのリソース生成
    CreateSwapChainAndResource();


    /// DSVの生成とステートの設定
    CreateDSVAndSettingState();


    /// フェンスとイベントの生成
    CreateFenceAndEvent();


    /// ビューポートとシザー矩形の設定
    SetViewportAndScissorRect();


    /// DXCの初期化
    CreateDirectXShaderCompiler();

    ///// D3D11デバイス群の生成
    //CreateD3D11Device();


    ///// Direct2Dのファクトリの生成
    //CreateD2D1Factory();


    ///// D2D1デバイスコンテキストの生成
    //CreateID2D1DeviceContext();


    ///// D2D1レンダーターゲットの生成
    //CreateD2DRenderTarget();
}

void DirectX12::PresentDraw()
{
    // バックバッファのインデックスを取得
    backBufferIndex_ = swapChain_->GetCurrentBackBufferIndex();

    // リソースバリアの設定
    SetResourceBarrier(D3D12_RESOURCE_BARRIER_TYPE_TRANSITION, D3D12_RESOURCE_BARRIER_FLAG_NONE,
        swapChainResources_[backBufferIndex_].Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);


    /// 描画先のRTV/DSVの設定
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
    commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex_], false, &dsvHandle);

    // 画面全体のクリア
    commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex_], clearColor_, 0, nullptr);
    // 指定した深度で画面全体をクリア
    commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    commandList_->RSSetViewports(1, &viewport_);        // Viewportを設定
    commandList_->RSSetScissorRects(1, &scissorRect_);        // Scissorを設定
}

void DirectX12::PostDraw()
{
    /// 描く状態から画面に映す状態に遷移
    SetResourceBarrier(D3D12_RESOURCE_BARRIER_TYPE_TRANSITION, D3D12_RESOURCE_BARRIER_FLAG_NONE,
        swapChainResources_[backBufferIndex_].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);

    /// コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseする
    hr_ = commandList_->Close();
    assert(SUCCEEDED(hr_) && "コマンドリストのクローズに失敗");


    /// GPUにコマンドリストの実行を行わせる
    Microsoft::WRL::ComPtr<ID3D12CommandList> commandLists[] = { commandList_.Get() };
    commandQueue_->ExecuteCommandLists(1, commandLists->GetAddressOf());


    /// GPUとISに画面の交換を行うよう通知する
    swapChain_->Present(1, 0);


    /// Fenceの値を更新
    fenceValue_++;


    /// GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
    commandQueue_->Signal(fence_.Get(), fenceValue_);

    /// GetCompletedValueの初期値はFence作成時に渡した初期値
    if (fence_->GetCompletedValue() < fenceValue_)
    {
        // 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
        fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
        // イベントを待つ
        WaitForSingleObject(fenceEvent_, INFINITE);
    }


    /// フレームレート固定
    pFramerate_->FixFramerate();


    /// 次のフレーム用のコマンドリストを準備
    hr_ = commandAllocator_->Reset();
    assert(SUCCEEDED(hr_));
    hr_ = commandList_->Reset(commandAllocator_.Get(), nullptr);
    assert(SUCCEEDED(hr_));

}

void DirectX12::CopyRTV()
{
    /// レンダーターゲットからコピー元状態にする
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = swapChainResources_[backBufferIndex_].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList_->ResourceBarrier(1, &barrier);


    D3D12_RESOURCE_BARRIER destBarrier = {};
    destBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    destBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    destBarrier.Transition.pResource = gameScreenResource_.Get();
    destBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    destBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
    destBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList_->ResourceBarrier(1, &destBarrier);


    D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
    srcLocation.pResource = swapChainResources_[backBufferIndex_].Get();
    srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    srcLocation.SubresourceIndex = 0; // コピーするサブリソースインデックス

    D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
    dstLocation.pResource = gameScreenResource_.Get();
    dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dstLocation.SubresourceIndex = 0;

    /// コピー範囲
    D3D12_BOX srcBox = {};
    srcBox.left = 0;
    srcBox.top = 0;
    srcBox.front = 0;
    srcBox.right = static_cast<UINT>(viewport_.Width);
    srcBox.bottom = static_cast<UINT>(viewport_.Height);
    srcBox.back = 1;

    commandList_->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, &srcBox);

    /// リソースバリアを戻す
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = swapChainResources_[backBufferIndex_].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList_->ResourceBarrier(1, &barrier);

    destBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    destBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    destBarrier.Transition.pResource = gameScreenResource_.Get();
    destBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    destBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    destBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList_->ResourceBarrier(1, &destBarrier);


    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
    // 画面全体のクリア
    commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex_], &editorBG_.x, 0, nullptr);
    // 指定した深度で画面全体をクリア
    commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

DirectX12::~DirectX12()
{
    const UINT64 fenceSignalValue = ++fenceValue_;
    ID3D12Fence* fence = fence_.Get();

    commandQueue_->Signal(fence, fenceSignalValue);
    if (fence->GetCompletedValue() < fenceSignalValue)
    {
        fence->SetEventOnCompletion(fenceSignalValue, fenceEvent_);
        WaitForSingleObject(fenceEvent_, INFINITE);
    }
    //auto ref = device_->Release();

    pSRVManager_->Deallocate(gameWndSrvIndex_);
}