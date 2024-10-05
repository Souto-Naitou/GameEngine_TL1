#pragma once

#include <BaseConfiguratorForEnum.h>

#include <utility>

#include <d3d12.h>

/// <summary>
/// ブレンドモード関連
/// </summary>
namespace BlendMode
{
    enum class BlendModes
    {
        None,       // !< ブレンドなし
        Normal,     // !< 通常αブレンド。デフォルト
        Add,        // !< 加算
        Subtract,   // !< 減算
        Multiply,   // !< 乗算
        Screen,     // !< スクリーン

        COUNT,      // !< カウント用
    };

    /// <summary>
    /// 設定クラス
    /// </summary>
    class Configurator : public BaseConfiguratorForEnum<BlendModes, D3D12_BLEND_DESC>
    {
    public:
        void Initialize(BlendModes _mode);
        void Apply(BlendModes _mode) override;
    };
}