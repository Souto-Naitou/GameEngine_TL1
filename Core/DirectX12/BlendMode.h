#pragma once

#include <BaseConfigurator.h>

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
    class Configurator : public BaseConfigurator<BlendModes>
    {
    public:
        void Initialize(BlendModes _mode);
        void ApplyMode(BlendModes _mode);

        inline D3D12_BLEND_DESC GetDesc() const { return blendDesc_; }

    private:
        D3D12_BLEND_DESC blendDesc_;
    };
}