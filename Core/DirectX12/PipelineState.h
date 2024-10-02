#pragma once

#include <BaseConfigurator.h>
#include <BlendMode.h>

#include <memory>

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
        void Initialize(Types _type);


    private:
        std::unique_ptr<BlendMode::Configurator> pBlendModeCfg_;
    };

}
