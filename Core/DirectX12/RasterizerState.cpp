#include <RasterizerState.h>

void RasterizerState::Configurator::Initialize()
{
    /// RasterizerStateの設定
// 裏面（時計回り）を表示しない (背面カリング)
    current_.CullMode = D3D12_CULL_MODE_BACK;
    // 三角形の中を塗りつぶす
    current_.FillMode = D3D12_FILL_MODE_SOLID;
}