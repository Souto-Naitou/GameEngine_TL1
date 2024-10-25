#include "DirectX12.h"

#include "Logger.h"             // ログ出力ラッパークラス
#include "ConvertString.h"      // wideとの互換
#include "Win32Application.h"   // Window関連
#include "DX12Helper.h"         // ヘルパー

#include <cassert>
#include <format>

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


    // ウィンドウハンドルを取得
    hwnd_ = Win32Application::GetInstance()->GetHwnd();

    hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
    assert(SUCCEEDED(hr_) && "DXGIファクトリの生成に失敗");


    /// アダプタ
    ChooseAdapter();


    /// デバイス生成
    DX12Helper::CreateDevice(device_, useAdapter_);


    /// エラー時停止処理
    DX12Helper::PauseError(device_, infoQueue_);


    /// Descriptorのサイズを取得 (動的に変わらないもの)
    kDescriptorSizeDSV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    kDescriptorSizeRTV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    kDescriptorSizeDSV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


    /// 出力ウィンドウに初期化完了を出力
    Log(std::format("DirectX12 Initialized.\n"));


    /// コマンド系を生成
    CreateCommandResources();


    /// スワップチェーンの生成とRTVのリソース生成
    CreateSwapChainAndResource();



}