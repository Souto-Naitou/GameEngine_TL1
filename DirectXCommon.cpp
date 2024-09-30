#include "DirectXCommon.h"
#include <cassert>
#include <format>
#include "Logger.h"
#include "ConvertString/ConvertString.h"
#include "DirectXCommonHelper.h"
#include "WinApp.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")

int DirectXCommon::Initialize()
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
    hwnd_ = WinApp::GetInstance()->GetHwnd();

    /// DXGIファクトリーの生成
    hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
    assert(SUCCEEDED(hr_) && "DXGIファクトリーの生成に失敗");

    /// アダプタの選択
    for (UINT i = 0;
        dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND;
        i++)
    {
        // アダプタの情報を取得
        DXGI_ADAPTER_DESC3 adapterDesc{};
        
        hr_ = useAdapter_->GetDesc3(&adapterDesc);
        assert(SUCCEEDED(hr_) && "アダプター情報の取得に失敗");
        // ソフトウェアアダプタではなければ採用する
        if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
        {
            // 採用したアダプタの情報をログに出力
            Log(std::format("Use Adapter:{}\n", ConvertString(adapterDesc.Description)));
            break;
        }
        useAdapter_ = nullptr;
    }
    assert(useAdapter_ != nullptr && "アダプタが見つかりません");

    /// ID3D12Deviceの生成
    CreateDevice(device_, useAdapter_);

    /// エラー対処
    PauseError(device_, infoQueue_);

    const uint32_t kDescriptorSizeSRV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    const uint32_t kDescriptorSizeRTV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    const uint32_t kDescriptorSizeDSV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

    // 出力ウインドウへ文字入力
    Log(std::format("Hello, {}\n", "DirectX!"));

    /// コマンドキューを生成
    hr_ = device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
    assert(SUCCEEDED(hr_) && "コマンドキューの生成に失敗");
    if (!commandQueue_) return -1;

    /// コマンドアロケータを生成
    hr_ = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
    assert(SUCCEEDED(hr_) && "コマンドアロケータの生成に失敗");
    if (!commandAllocator_) return -1;

    /// コマンドリストを生成する
    hr_ = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
    assert(SUCCEEDED(hr_) && "コマンドリストの生成がうまくいかなかったので起動できない");

    /// スワップチェーンの作成
    swapChainDesc_.Width = clientWidth_;                             // 画面の幅。ウィンドウのクライアント領域を同じものにしておく
    swapChainDesc_.Height = clientHeight_;                           // 画面の高さ。ウィンドウのクライアント領域を同じものにしておく
    swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;              // 色の形式
    swapChainDesc_.SampleDesc.Count = 1;                             // マルチサンプルしない
    swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // 描画のターゲットとして利用する
    swapChainDesc_.BufferCount = 2;                                  // ダブルバッファ
    swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;       // モニタにうつしたら、中身を破棄

    /// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
    hr_ = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), hwnd_, &swapChainDesc_, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
    assert(SUCCEEDED(hr_));
    /// ディスクリプタヒープの生成
    rtvDescriptorHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
    srvDescriptorHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);

    /// SwapChainからResourceを引っ張ってくる
    hr_ = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
    assert(SUCCEEDED(hr_) && "SwapChainからリソースを取得できませんでした [0]");
    hr_ = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
    assert(SUCCEEDED(hr_) && "SwapChainからリソースを取得できませんでした [1]");

    /// RTVの設定
    rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

    /// 初期値0でFenceを作る
    hr_ = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    assert(SUCCEEDED(hr_) && "フェンス生成に失敗");

    /// FenceのSignalを待つためのイベントを作成する
    fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
    assert(fenceEvent_ != nullptr);

    /// dxcCompilerを初期化
    hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
    assert(SUCCEEDED(hr_) && "DirectX Shader Compilerの初期化に失敗しました [DxcUtils]");
    hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
    assert(SUCCEEDED(hr_) && "DirectX Shader Compilerの初期化に失敗しました [DxcCompiler]");

    /// 現時点でincludeはしないが、includeに対応するための設定を行っておく
    hr_ = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
    assert(SUCCEEDED(hr_) && "includeに対応する設定に失敗しました");

    /// ディスクリプタテーブルのディスクリプタレンジを設定する (SRVを通じてシェーダにリソースへのアクセスを提供・t0)
    descriptorRange_[0].BaseShaderRegister = 0; // 0から始まる
    descriptorRange_[0].NumDescriptors = 1; // 数は1つ
    descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
    descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

    /// RootSignature作成
    descriptionRootSignature_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    descriptionRootSignature_.pParameters = rootParameters_;                              // ルートパラメータ配列へのポインタ
    descriptionRootSignature_.NumParameters = _countof(rootParameters_);                  // 配列の長さ

    /// RootParameter作成。複数設定できるので配列。
    rootParameters_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使う
    rootParameters_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使う
    rootParameters_[0].Descriptor.ShaderRegister = 0;                                    // レジスタ番号０とバインド

    rootParameters_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使う
    rootParameters_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;                // VertexShaderで使う
    rootParameters_[1].Descriptor.ShaderRegister = 0;                                    // レジスタ番号０とバインド

    rootParameters_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;       // DescriptorTableを使う
    rootParameters_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使う
    rootParameters_[2].DescriptorTable.pDescriptorRanges = descriptorRange_;              // Tableの中身の配列を指定
    rootParameters_[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);  // Tableで利用する数

    rootParameters_[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使用する
    rootParameters_[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使用する
    rootParameters_[3].Descriptor.ShaderRegister = 1;                                    // レジスタ番号1を使用する

    staticSamplers_[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                         // BilinearFilter
    staticSamplers_[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                       // 0 ~ 1の範囲外をリピート
    staticSamplers_[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers_[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers_[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;                     // 比較しない
    staticSamplers_[0].MaxLOD = D3D12_FLOAT32_MAX;                                       // ありったけのーを使う
    staticSamplers_[0].ShaderRegister = 0;                                               // レジスタ番号0を使用する
    staticSamplers_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderを使う

    descriptionRootSignature_.pStaticSamplers = staticSamplers_;
    descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers_);

    /// シリアライズしてバイナリにする
    hr_ = D3D12SerializeRootSignature(&descriptionRootSignature_,
        D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
    if (FAILED(hr_))
    {
        Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
        assert(false);
    }

    // バイナリをもとに生成
    hr_ = device_->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
        signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(hr_));

    /// InputLayout
    inputElementDescs_[0].SemanticName = "POSITION";
    inputElementDescs_[0].SemanticIndex = 0;
    inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    inputElementDescs_[1].SemanticName = "TEXCOORD";
    inputElementDescs_[1].SemanticIndex = 0;
    inputElementDescs_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputElementDescs_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    inputElementDescs_[2].SemanticName = "NORMAL";
    inputElementDescs_[2].SemanticIndex = 0;
    inputElementDescs_[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDescs_[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
    inputLayoutDesc_.NumElements = _countof(inputElementDescs_);

    /// BlendStateの設定
    // すべての色要素を書き込む
    blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    /// RasterizerStateの設定
    // 裏面（時計回り）を表示しない (背面カリング)
    rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
    // 三角形の中を塗りつぶす
    rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

    /// ShaderをCompileする
    vertexShaderBlob_ = CompileShader(L"Resources/shaders/Object3D.VS.hlsl", L"vs_6_6", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
    assert(vertexShaderBlob_ != nullptr);

    pixelShaderBlob_ = CompileShader(L"Resources/shaders/Object3D.PS.hlsl", L"ps_6_6", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
    assert(pixelShaderBlob_ != nullptr);

    // DespStencilResource
    depthStencilResource_ = CreateDepthStencilTextureResource(device_, clientWidth_, clientHeight_);
    // DSV用のヒープでディスクリプタの数は1。DSVはShader内で触るものではないため、ShaderVisibleはfalse
    dsvDescriptorHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

    /// DSVの設定
    dsvDesc_.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format。基本的にはResourceに合わせる
    dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture
    // DSVHeapの戦闘にDSVを作る
    device_->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc_, dsvDescriptorHeap_.Get()->GetCPUDescriptorHandleForHeapStart());

    /// DepthStencilStateの設定
    // Depthの機能を有効にする
    depthStencilDesc_.DepthEnable = true;
    // 書き込みする
    depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    // 比較関数はLessEqual。つまり、近ければ描画される
    depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

    /// PSOを生成する
    graphicsPipelineStateDesc_.pRootSignature = rootSignature_.Get();	// RootSignature
    graphicsPipelineStateDesc_.InputLayout = inputLayoutDesc_;	// InputLayout
    graphicsPipelineStateDesc_.VS = { vertexShaderBlob_.Get()->GetBufferPointer(), vertexShaderBlob_.Get()->GetBufferSize() }; // VertexShader
    graphicsPipelineStateDesc_.PS = { pixelShaderBlob_.Get()->GetBufferPointer(), pixelShaderBlob_.Get()->GetBufferSize() }; // PixelShader
    graphicsPipelineStateDesc_.BlendState = blendDesc_; // BlendState
    graphicsPipelineStateDesc_.RasterizerState = rasterizerDesc_; // RasterizerState
    // 書き込むRTVの情報
    graphicsPipelineStateDesc_.NumRenderTargets = 1;
    graphicsPipelineStateDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    // 利用するトポロジ（形状）のタイプ。三角形
    graphicsPipelineStateDesc_.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // どのように画面に色を打ち込むかの設定（気にしなくて良い）
    graphicsPipelineStateDesc_.SampleDesc.Count = 1;
    graphicsPipelineStateDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    // DepthStencilの設定
    graphicsPipelineStateDesc_.DepthStencilState = depthStencilDesc_;
    graphicsPipelineStateDesc_.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    /// 実際に生成
    hr_ = device_->CreateGraphicsPipelineState(&graphicsPipelineStateDesc_,
        IID_PPV_ARGS(&graphicsPipelineState_));
    assert(SUCCEEDED(hr_));
    if (!graphicsPipelineState_) return -1;

    return 0;
}

void DirectXCommon::Finalize()
{
    CloseHandle(fenceEvent_);
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DirectXCommon::CreateDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, D3D12_DESCRIPTOR_HEAP_TYPE _heapType, UINT _numDescriptors, bool _shaderVisible)
{
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
    descriptorHeapDesc.Type = _heapType;
    descriptorHeapDesc.NumDescriptors = _numDescriptors;
    descriptorHeapDesc.Flags = _shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT hr = _device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
    return descriptorHeap;
}

Microsoft::WRL::ComPtr<IDxcBlob> DirectXCommon::CompileShader(
    const std::wstring& filePath, 
    const wchar_t* profile, 
    const Microsoft::WRL::ComPtr<IDxcUtils>& dxcUtils, 
    const Microsoft::WRL::ComPtr<IDxcCompiler3>& dxcCompiler, 
    const Microsoft::WRL::ComPtr<IDxcIncludeHandler>& includeHandler
)
{
    /// 1. hlslファイルを読み込む

    // これからシェーダーをコンパイルする旨をログに出す
    Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
    // hlslファイルを読む
    Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
    HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
    // 読めなかったら止める
    assert(SUCCEEDED(hr));
    // 読み込んだファイルの内容を設定する
    DxcBuffer shaderSourceBuffer;
    shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
    shaderSourceBuffer.Size = shaderSource->GetBufferSize();
    shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF-8の文字コードであることを通知

    /// 2. Compileする
    LPCWSTR arguments[] = {
        filePath.c_str(),			// コンパイル対象のhlslファイル名
        L"-E", L"main",				// エントリーポイントの指定。基本的にmain以外にはしない
        L"-T", profile,				// ShaderProfileの設定
        L"-Zi", L"-Qembed_debug",	// デバッグ用の情報を埋め込む
        L"-Od",						// 最適化を外しておく
        L"-Zpr",					// メモリレイアウトは行優先
    };
    // 実際にShaderをコンパイルする
    Microsoft::WRL::ComPtr<IDxcResult> shaderResult = nullptr;
    hr = dxcCompiler->Compile(
        &shaderSourceBuffer,		// 読み込んだファイル
        arguments,					// コンパイルオプション
        _countof(arguments),		// コンパイルオプションの数
        includeHandler.Get(),				// includeが含まれた諸々
        IID_PPV_ARGS(&shaderResult)	// コンパイル結果
    );
    // コンパイルエラーではなくdxcが起動できないなど致命的な状況
    assert(SUCCEEDED(hr));

    /// 3. 警告・エラーが出ていないか確認する
    Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError = nullptr;
    shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
    if (shaderError != nullptr && shaderError->GetStringLength() != 0)
    {
        Log(shaderError->GetStringPointer());
        // 警告・エラーダメゼッタイ
        assert(false);
    }

    /// 4. Compile結果を受け取って返す

    // コンパイル結果から実行用のバイナリ部分を取得
    Microsoft::WRL::ComPtr<IDxcBlob> shaderBlob = nullptr;
    hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
    assert(SUCCEEDED(hr));
    // 成功したログを出す
    Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
    // 実行用のバイナリを返却
    return shaderBlob;
}

Microsoft::WRL::ComPtr<ID3D12Resource> DirectXCommon::CreateDepthStencilTextureResource(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, int32_t _width, int32_t _height)
{
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Width = _width;
    resourceDesc.Height = _height;
    resourceDesc.MipLevels = 1; // mipmapの数
    resourceDesc.DepthOrArraySize = 1;	// 奥行き or 配列Textureの配列数
    resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

    // 利用するHeapの設定
    D3D12_HEAP_PROPERTIES heapProperties{};
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAMに

    // 深度値のクリア設定
    D3D12_CLEAR_VALUE depthClearValue{};
    depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
    depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resourceと合わせる

    // Resourceの生成
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
