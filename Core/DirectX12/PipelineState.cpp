#include "PipelineState.h"

PipelineState::Configurator::Configurator()
{
    // ブレンドモード
    pBlendModeCfg_ = std::make_unique<BlendMode::Configurator>();

    pInputLayoutDesc_ = std::make_unique<InputLayout::Configurator>();

    pRootSignature_ = std::make_unique<RootSignature::Configurator>();
}

void PipelineState::Configurator::Initialize(Types _type, ID3D12Device* _device)
{
    pBlendModeCfg_->Initialize(BlendMode::BlendModes::None);
    pInputLayoutDesc_->Initialize();
    pRootSignature_->Initialize(_device);


    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};
    // RootSignature
    pipelineStateDesc.pRootSignature = pRootSignature_->Get();
    // InputLayout
    pipelineStateDesc.InputLayout = pInputLayoutDesc_->Get();
    // VertexShader
    pipelineStateDesc.VS = { vertexShaderBlob_.Get()->GetBufferPointer(), vertexShaderBlob_.Get()->GetBufferSize() };
    // PixelShader
    pipelineStateDesc.PS = { pixelShaderBlob_.Get()->GetBufferPointer(), pixelShaderBlob_.Get()->GetBufferSize() };
    // BlendState
    pipelineStateDesc.BlendState = pBlendModeCfg_->Get();
    // RasterizerState
    pipelineStateDesc.RasterizerState = rasterizerDesc_;
    // 書き込むRTVの情報
    pipelineStateDesc.NumRenderTargets = 1;
    pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    // 利用するトポロジ（形状）のタイプ。三角形
    pipelineStateDesc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    // どのように画面に色を打ち込むかの設定（気にしなくて良い）
    pipelineStateDesc.SampleDesc.Count = 1;
    pipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    // DepthStencilの設定
    pipelineStateDesc.DepthStencilState = depthStencilDesc_;
    pipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
}
