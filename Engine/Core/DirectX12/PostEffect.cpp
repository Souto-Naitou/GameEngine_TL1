#include "PostEffect.h"

#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Core/Win32/WinSystem.h>

void PostEffect::Initialize()
{
    // インスタンスの取得
    ObtainInstances();

    // 描画用コマンドリストの生成
    CreateCommandList();

    // レンダーテクスチャの生成
    CreateRenderTexture();

    // SRVの生成
    CreateSRV();

    // ルートシグネチャの生成
    CreateRootSignature();

    // パイプラインステートの生成
    CreatePipelineState();
}

void PostEffect::NewFrame()
{
    /// 描画先のRTV/DSVの設定
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
    commandListMain_->OMSetRenderTargets(1, &rtvHandle_, false, &dsvHandle);

    // 画面全体のクリア
    commandListMain_->ClearRenderTargetView(rtvHandle_, &editorBG_.x, 0, nullptr);

    // 指定した深度で画面全体をクリア
    commandListMain_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // 描画先のビューを設定
    commandListMain_->RSSetViewports(1, &viewport_);
    commandListMain_->RSSetScissorRects(1, &scissorRect_);
}

void PostEffect::Draw()
{
    // コマンドリストの設定
    rtvHandleSwapChain_ = pDx12_->GetRTVHandle()[pDx12_->GetBackBufferIndex()];
    DX12Helper::CommandListCommonSetting(commandListForDraw_.Get(), &rtvHandleSwapChain_);

    DX12Helper::ChangeStateResource(
        commandListForDraw_,
        renderTexture_,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );

    commandListForDraw_->SetGraphicsRootSignature(rootSignature_.Get());
    commandListForDraw_->SetPipelineState(pso_.Get());
    commandListForDraw_->SetGraphicsRootDescriptorTable(0, rtvHandleGpu_);

    commandListForDraw_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandListForDraw_->DrawInstanced(3, 1, 0, 0);

    DX12Helper::ChangeStateResource(
        commandListForDraw_,
        renderTexture_,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );
}

void PostEffect::PostDraw()
{
    commandAllocator_->Reset();
    commandListForDraw_->Reset(commandAllocator_.Get(), nullptr);
}

void PostEffect::OnResize()
{
    pSRVManager_->Deallocate(renderSRVIndex_);
    renderTexture_.resource.Reset();
    renderTexture_.state = D3D12_RESOURCE_STATE_PRESENT;
}

void PostEffect::OnResizedBuffers()
{
    CreateRenderTexture();
    CreateSRV();
}

void PostEffect::CreateRenderTexture()
{
    renderTexture_.resource = DX12Helper::CreateRenderTextureResource(
        pDevice_,
        WinSystem::clientWidth,
        WinSystem::clientHeight,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        pDx12_->GetEditorBGColor()
    );
    renderTexture_.state = D3D12_RESOURCE_STATE_RENDER_TARGET;

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

    if (rtvHandle_.ptr == 0)
    {
        rtvHeapIndex_ = rtvHeapCounter_->Allocate("PostEffectRTV");
        rtvHandle_ = rtvHeapCounter_->GetRTVHandle(rtvHeapIndex_);
    }

    pDevice_->CreateRenderTargetView(
        renderTexture_.resource.Get(),
        &rtvDesc,
        rtvHandle_
    );
    renderTexture_.resource->SetName(L"RenderTexture");
}

void PostEffect::CreateSRV()
{
    renderSRVIndex_ = pSRVManager_->Allocate();

    pSRVManager_->CreateForTexture2D(
        renderSRVIndex_,
        renderTexture_.resource.Get(),
        DXGI_FORMAT_R8G8B8A8_UNORM,
        1
    );

    rtvHandleGpu_ = pSRVManager_->GetGPUDescriptorHandle(renderSRVIndex_);
}

void PostEffect::ObtainInstances()
{
    /// 必要なインスタンスを取得
    pDx12_ = DirectX12::GetInstance();
    rtvHeapCounter_ = pDx12_->GetRTVHeapCounter();
    pDevice_ = pDx12_->GetDevice();
    pSRVManager_ = SRVManager::GetInstance();
    commandListMain_ = pDx12_->GetCommandList();
    viewport_ = pDx12_->GetViewport();
    scissorRect_ = pDx12_->GetScissorRect();
    dsvHeap_ = pDx12_->GetDSVDescriptorHeap();
    rtvHeap_ = pDx12_->GetRTVDescriptorHeap();
    editorBG_ = pDx12_->GetEditorBGColor();
}

void PostEffect::CreateRootSignature()
{
    D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
    descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
    descriptorRange[0].NumDescriptors = 1; // 数は1つ
    descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
    descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

    /// RootSignature作成
    D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
    descriptionRootSignature.Flags =
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // RootParameter作成。複数設定できるので配列
    D3D12_ROOT_PARAMETER rootParameters[1] = {};
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;       // DescriptorTableを使う
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使う
    rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;              // Tableの中身の配列を指定
    rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);  // Tableで利用する数


    descriptionRootSignature.pParameters = rootParameters;                              // ルートパラメータ配列へのポインタ
    descriptionRootSignature.NumParameters = _countof(rootParameters);                  // 配列の長さ

    D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
    staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;              // 異方性フィルタリング
    staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].MipLODBias = 0.0f;                                    // ミップマップのオフセット
    staticSamplers[0].MaxAnisotropy = 16;                                   // 最大異方性
    staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;         // 比較なし
    staticSamplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE; // ボーダーカラー
    staticSamplers[0].MinLOD = 0.0f;                                        // 最小ミップレベル
    staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                           // 最大ミップレベル
    staticSamplers[0].ShaderRegister = 0;                                   // サンプラーのレジスタ番号
    staticSamplers[0].RegisterSpace = 0;                                    // レジスタスペース
    staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;     // ピクセルシェーダーで使用

    descriptionRootSignature.pStaticSamplers = staticSamplers;
    descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

    // シリアライズしてバイナリにする
    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(
            "Object3dSystem",
            "CreateRootSignature",
            reinterpret_cast<char*>(errorBlob->GetBufferPointer())
        );

        assert(false);
    }
    // バイナリをもとに生成
    hr = pDevice_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(hr));

}

void PostEffect::CreatePipelineState()
{
    ID3D12Device* device = pDx12_->GetDevice();
    IDxcUtils* dxcUtils = pDx12_->GetDxcUtils();
    IDxcCompiler3* dxcCompiler = pDx12_->GetDxcCompiler();
    IDxcIncludeHandler* includeHandler = pDx12_->GetIncludeHandler();

    inputLayoutDesc_.pInputElementDescs = nullptr;
    inputLayoutDesc_.NumElements = 0;

    /// BlendStateの設定
    D3D12_BLEND_DESC blendDesc{};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


    /// RasterizerStateの設定
    rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
    rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerDesc_.MultisampleEnable = TRUE;  // アンチエイリアス有効化
    rasterizerDesc_.AntialiasedLineEnable = TRUE;  // ラインのアンチエイリアス有効化

    /// ShaderをCompileする
    vertexShaderBlob_ = DX12Helper::CompileShader(kVertexShaderPath, L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
    assert(vertexShaderBlob_ != nullptr);

    pixelShaderBlob_ = DX12Helper::CompileShader(kPixelShaderPath, L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
    assert(pixelShaderBlob_ != nullptr);

    /// PSOを生成する
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
    graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();    // RootSignature
    graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;    // InputLayout
    graphicsPipelineStateDesc.VS = { vertexShaderBlob_.Get()->GetBufferPointer(), vertexShaderBlob_.Get()->GetBufferSize() };
    graphicsPipelineStateDesc.PS = { pixelShaderBlob_.Get()->GetBufferPointer(), pixelShaderBlob_.Get()->GetBufferSize() };
    graphicsPipelineStateDesc.BlendState = blendDesc;            // BlendState
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;    // RasterizerState
    // 書き込むRTVの情報
    graphicsPipelineStateDesc.NumRenderTargets = 1;
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    // 利用するトポロジ（形状）のタイプ。三角形
    graphicsPipelineStateDesc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // どのように画面に色を打ち込むかの設定（気にしなくて良い）
    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    // DepthStencilの設定
    graphicsPipelineStateDesc.DepthStencilState = {};
    graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    // 実際に生成
    HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pso_));
    if (FAILED(hr))
    {
        Logger::GetInstance()->LogError(
            "PostEffect",
            "CreatePipelineState",
            "Failed to create pipeline state"
        );
        assert(false);
    }

    return;
}

void PostEffect::CreateCommandList()
{
    pDevice_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator_.GetAddressOf()));
    pDevice_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(commandListForDraw_.GetAddressOf()));
}
