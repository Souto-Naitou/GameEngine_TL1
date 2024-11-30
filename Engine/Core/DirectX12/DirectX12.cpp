#include "DirectX12.h"

#include <DebugTools/Logger/Logger.h>                     // ログ出力ラッパークラス
#include <Utility/ConvertString/ConvertString.h>              // wideとの互換
#include <Core/Win32/Win32Application.h>        // Window関連
#include <Core/DirectX12/Helper/DX12Helper.h>   // ヘルパー

#include <cassert>
#include <format>
#include <iostream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")


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

    pFramerate_ = FrameRate::GetInstance();
    pFramerate_->Initialize();

    // ウィンドウハンドルを取得
    hwnd_ = Win32Application::GetInstance()->GetHwnd();

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
    commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex_], false, nullptr);
    commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex_], false, &dsvHandle);


    // 画面全体のクリア
    commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex_], clearColor_, 0, nullptr);
    // 指定した深度で画面全体をクリア
    commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap_.Get() };
    commandList_->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

    commandList_->RSSetViewports(1, &viewport_);                    // Viewportを設定
    commandList_->RSSetScissorRects(1, &scissorRect_);              // Scissorを設定
    // RootSignatureを設定。PSOに設定しているけど別途設定が必要
    //commandList_->SetGraphicsRootSignature(rootSignature.Get());
    //commandList_->SetPipelineState(graphicsPipelineState.Get());  // PSOを設定
    //commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);    // VBVを設定
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
}
