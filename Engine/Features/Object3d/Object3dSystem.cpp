#include "Object3dSystem.h"
#include <DebugTools/Logger/Logger.h>
#include <cassert>
#include <Core/DirectX12/Helper/DX12Helper.h>

Object3dSystem::Object3dSystem()
{
    pDx12_ = DirectX12::GetInstance();
}

void Object3dSystem::Initialize()
{
    ObjectSystemBase::Initialize();

    CreateRootSignature();
    CreateMainPipelineState();
    CreateDepthPipelineState();
}

void Object3dSystem::DepthDrawSetting()
{
    ID3D12GraphicsCommandList* commandList = pDx12_->GetCommandList();

    /// ルートシグネチャをセットする
    commandList->SetGraphicsRootSignature(rootSignature_.Get());

    /// グラフィックスパイプラインステートをセットする
    commandList->SetPipelineState(psoEarlyZ_.Get());

    /// プリミティブトポロジーをセットする
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3dSystem::MainDrawSetting()
{
    ID3D12GraphicsCommandList* commandList = pDx12_->GetCommandList();

    /// ルートシグネチャをセットする
    commandList->SetGraphicsRootSignature(rootSignature_.Get());

    /// グラフィックスパイプラインステートをセットする
    commandList->SetPipelineState(psoMain_.Get());

    /// プリミティブトポロジーをセットする
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3dSystem::DrawCall()
{
    auto record = [&](ID3D12GraphicsCommandList* _commandList)
    {
        /// コマンドリストの設定
        DX12Helper::CommandListCommonSetting(_commandList, rtvHandle_);

        /// ルートシグネチャをセットする
        _commandList->SetGraphicsRootSignature(rootSignature_.Get());

        /// グラフィックスパイプラインステートをセットする
        _commandList->SetPipelineState(psoEarlyZ_.Get());

        /// プリミティブトポロジーをセットする
        _commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        for(auto& data : commandListDatas_)
        {
            for(auto& cbuffer : data.cbuffers)
            {
                auto& [key, value] = cbuffer;
                _commandList->SetGraphicsRootConstantBufferView(key, value->GetGPUVirtualAddress());
            }

            data.model->Draw(_commandList);
        }

        /// ルートシグネチャをセットする
        _commandList->SetGraphicsRootSignature(rootSignature_.Get());

        /// グラフィックスパイプラインステートをセットする
        _commandList->SetPipelineState(psoMain_.Get());

        /// プリミティブトポロジーをセットする
        _commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        for(auto& data : commandListDatas_)
        {
            for(auto& cbuffer : data.cbuffers)
            {
                auto& [key, value] = cbuffer;
                _commandList->SetGraphicsRootConstantBufferView(key, value->GetGPUVirtualAddress());
            }

            data.model->Draw(_commandList);
        }
    };

    worker_ = std::async(std::launch::async, record, commandList_.Get());
}

void Object3dSystem::Sync()
{
    worker_.get();
    commandListDatas_.clear();
}

void Object3dSystem::AddCommandListData(CommandListData& _data)
{
    commandListDatas_.emplace_back(_data);
}

void Object3dSystem::CreateRootSignature()
{
    ID3D12Device* device = pDx12_->GetDevice();

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
    D3D12_ROOT_PARAMETER rootParameters[8] = {};
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使う
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使う
    rootParameters[0].Descriptor.ShaderRegister = 0;                                    // レジスタ番号０とバインド

    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使う
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;                // VertexShaderで使う
    rootParameters[1].Descriptor.ShaderRegister = 0;                                    // レジスタ番号０とバインド

    rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;       // DescriptorTableを使う
    rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使う
    rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;              // Tableの中身の配列を指定
    rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);  // Tableで利用する数

    rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使用する
    rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使用する
    rootParameters[3].Descriptor.ShaderRegister = 1;                                    // レジスタ番号1を使用する

    rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使用する
    rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使用する
    rootParameters[4].Descriptor.ShaderRegister = 2;                                    // レジスタ番号2を使用する

    rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使用する
    rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使用する
    rootParameters[5].Descriptor.ShaderRegister = 3;                                    // レジスタ番号3を使用する

    /// Lighting
    rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使用する
    rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使用する
    rootParameters[6].Descriptor.ShaderRegister = 4;                                    // レジスタ番号4を使用する

    /// PointLight
    rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使用する
    rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使用する
    rootParameters[7].Descriptor.ShaderRegister = 5;                                    // レジスタ番号5を使用する


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
    HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature,
        D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
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
    hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(hr));
}

void Object3dSystem::CreateMainPipelineState()
{
    ID3D12Device* device = pDx12_->GetDevice();
    IDxcUtils* dxcUtils = pDx12_->GetDxcUtils();
    IDxcCompiler3* dxcCompiler = pDx12_->GetDxcCompiler();
    IDxcIncludeHandler* includeHandler = pDx12_->GetIncludeHandler();
    uint32_t clientWidth = pDx12_->GetClientWidth();
    uint32_t clientHeight = pDx12_->GetClientHeight();

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

    // DespStencilResource
    Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = DX12Helper::CreateDepthStencilTextureResource(device, clientWidth, clientHeight);
    // DSV用のヒープでディスクリプタの数は1。DSVはShader内で触るものではないため、ShaderVisibleはfalse
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = DX12Helper::CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
    // DSVの設定
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format。基本的にはResourceに合わせる
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture
    // DSVHeapの先頭にDSVを作る
    device->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dsvDescriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart());
    // DepthStencilStateの設定
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
    // Depthの機能を有効にする
    depthStencilDesc.DepthEnable = true;
    // 書き込みする
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    // 比較関数はLessEqual。つまり、近ければ描画される
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

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
    graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
    graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    // 実際に生成
    HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&psoMain_));
    if (FAILED(hr)) [[unlikely]]
    {
        Logger::GetInstance()->LogError(
            "Object3dSystem",
            __func__,
            "Failed to create pipeline state"
        );
        assert(false);
    }
    return;
}

void Object3dSystem::CreateDepthPipelineState()
{
    ID3D12Device* device = pDx12_->GetDevice();
    uint32_t clientWidth = pDx12_->GetClientWidth();
    uint32_t clientHeight = pDx12_->GetClientHeight();


    /// BlendDesc
    D3D12_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].RenderTargetWriteMask = 0;


    // DespStencilResource
    Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = DX12Helper::CreateDepthStencilTextureResource(device, clientWidth, clientHeight);
    // DSV用のヒープでディスクリプタの数は1。DSVはShader内で触るものではないため、ShaderVisibleはfalse
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = DX12Helper::CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
    // DSVの設定
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format。基本的にはResourceに合わせる
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture
    // DSVHeapの先頭にDSVを作る
    device->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dsvDescriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart());

    /// DepthStencilStateの設定 (Early-Zのために変更)
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

    /// PSOを生成する
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
    graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();
    graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;
    graphicsPipelineStateDesc.VS = { vertexShaderBlob_.Get()->GetBufferPointer(), vertexShaderBlob_.Get()->GetBufferSize() };
    graphicsPipelineStateDesc.PS = { nullptr, 0 };
    graphicsPipelineStateDesc.BlendState = blendDesc;
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;

    // 書き込むRTVの情報
    graphicsPipelineStateDesc.NumRenderTargets = 0;

    // 利用するトポロジ（形状）のタイプ。三角形
    graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // どのように画面に色を打ち込むかの設定（気にしなくて良い）
    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

    // DepthStencilの設定
    graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
    graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    

    /// 生成
    HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&psoEarlyZ_));
    if (FAILED(hr)) [[unlikely]]
    {
        Logger::GetInstance()->LogError(
            "Object3dSystem",
            __func__,
            "Failed to create pipeline state"
        );
        assert(false);
    }

    return;
}