#include "DX12Helper.h"

#include <cassert>
#include <Logger.h>
#include <format>

void DX12Helper::CreateDevice(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<IDXGIAdapter4>& _adapter)
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


void DX12Helper::PauseError(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<ID3D12InfoQueue>& _infoQ)
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


Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DX12Helper::CreateDescriptorHeap(Microsoft::WRL::ComPtr<ID3D12Device>& _device, D3D12_DESCRIPTOR_HEAP_TYPE _heapType, UINT _numDescriptors, bool _shaderVisible)
{
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
    descriptorHeapDesc.Type = _heapType;
    descriptorHeapDesc.NumDescriptors = _numDescriptors;
    descriptorHeapDesc.Flags = _shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT hr = _device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
    return descriptorHeap;
}

Microsoft::WRL::ComPtr<ID3D12Resource> DX12Helper::CreateDepthStencilTextureResource(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, int32_t _width, int32_t _height)
{
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Width = _width;                                    // 幅
    resourceDesc.Height = _height;                                  // 高さ
    resourceDesc.MipLevels = 1;                                     // mipmapの数
    resourceDesc.DepthOrArraySize = 1;	                            // 奥行き or 配列Textureの配列数
    resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;            // フォーマット
    resourceDesc.SampleDesc.Count = 1;                              // サンプリング数
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;    // 2DTexture
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;   // DepthStencilとして使う通知


    /// 利用するHeapの設定
    D3D12_HEAP_PROPERTIES heapProperties{};
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;                  // VRAMに


    /// 深度値のクリア設定
    D3D12_CLEAR_VALUE depthClearValue{};
    depthClearValue.DepthStencil.Depth = 1.0f;                      // 1.0f(最大値)でクリア
    depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;         // フォーマット。Resourceと合わせる


    /// Resourceの生成
    Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
    HRESULT hr = _device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &depthClearValue,
        IID_PPV_ARGS(&resource)
    );
    assert(SUCCEEDED(hr));


    return resource;
}
