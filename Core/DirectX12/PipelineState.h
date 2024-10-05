#pragma once

#include <BaseConfigurator.h>
#include <BlendMode.h>
#include <RootSignature.h>
#include <InputLayout.h>
#include <Dxc.h>
#include <RasterizerState.h>
#include <DepthStencil.h>

#include <memory>
#include <d3d12.h>
#include <wrl.h>

namespace PipelineState
{
    /// <summary>
    /// 命名規則
    /// BlendMode_
    /// </summary>
    enum class Types
    {
        None,       // !< ブレンドなし
        Normal,     // !< 通常αブレンド。デフォルト
        Add,        // !< 加算
        Subtract,   // !< 減算
        Multiply,   // !< 乗算
        Screen,     // !< スクリーン
    };

    class Configurator : public BaseConfiguratorForEnum<PipelineState::Types, Microsoft::WRL::ComPtr<ID3D12PipelineState>>
    {
    public:
        Configurator();
        
        void Initialize(Types _type, ID3D12Device* _device);
        void Apply(Types _type);

    private:
        std::unique_ptr<BlendMode::Configurator> pBlendModeCfg_;
        std::unique_ptr<RootSignature::Configurator> pRootSignature_;
        std::unique_ptr<InputLayout::Configurator> pInputLayoutDesc_;
        std::unique_ptr<VertexShaderBlob::Configurator> pVertexShaderBlob_;
        std::unique_ptr<PixelShaderBlob::Configurator> pPixelShaderBlob_;
        std::unique_ptr<RasterizerState::Configurator> pRasterizerState_;
        std::unique_ptr<DepthStencil::Configurator> pDepthStencil_;
    };

}
