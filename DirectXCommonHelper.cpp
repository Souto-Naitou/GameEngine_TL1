#include "DirectXCommonHelper.h"
#include "Logger.h"
#include <format>
#include <cassert>

void CreateDevice(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<IDXGIAdapter4>& _adapter)
{
	/// D3D12Deviceの生成

	// 機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2", "12.1", "12.0" };
	// 高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		// 採用したアダプターでデバイスを生成
		HRESULT hr = D3D12CreateDevice(_adapter.Get(), featureLevels[i], IID_PPV_ARGS(&_device));
		// 指定した機能レベルでデバイスが生成できたかを確認
		if (SUCCEEDED(hr))
		{
			// 生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	// デバイスの生成がうまくいかなかったので起動できない
	assert(_device && "デバイスの生成に失敗");
	Log("Complete create D3D12Device!!!\n"); // 初期化完了のログを出力
}

void PauseError(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<ID3D12InfoQueue>& _infoQ)
{
#ifdef _DEBUG
	if (SUCCEEDED(_device->QueryInterface(IID_PPV_ARGS(&_infoQ))))
	{
		// やばいエラー時に止まる　
		_infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		// エラー時に止まる <- 解放忘れが判明したら、コメントアウト
		_infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		_infoQ->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		// 開放
		_infoQ->Release();

		// 抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			// Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		// 抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したメッセージの表示を制限する
		_infoQ->PushStorageFilter(&filter);
	}
#endif // _DEBUG
}
