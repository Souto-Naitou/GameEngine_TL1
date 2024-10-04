#pragma once

#include <BaseConfigurator.h>
#include <BlendMode.h>
#include <RootSignature.h>
#include <InputLayout.h>

#include <memory>
#include <d3d12.h>

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

    class Configurator : public BaseConfigurator<PipelineState::Types>
    {
    public:
        Configurator();
        
        void Initialize(Types _type, ID3D12Device* _device);


    private:
        std::unique_ptr<BlendMode::Configurator> pBlendModeCfg_;
        std::unique_ptr<RootSignature::Configurator> pRootSignature_;
        std::unique_ptr<InputLayout::Configurator> pInputLayoutDesc_;
    };

}
