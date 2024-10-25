#pragma once

#include <utility>

#include <d3d12.h>

/// <summary>
/// ブレンドモード関連
/// </summary>
class BlendMode
{
public:
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

    const D3D12_BLEND_DESC& GetBlendDesc() const { return blendDesc_; }

    void Initialize(BlendModes _mode);
    void Apply(BlendModes _mode);

private:
    D3D12_BLEND_DESC blendDesc_;
    BlendModes currentMode_;
};