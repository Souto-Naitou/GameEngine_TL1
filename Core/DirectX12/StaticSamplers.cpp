#include <StaticSamplers.h>

void StaticSamplers::Configurator::Initialize()
{
    current_[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                         // BilinearFilter
    current_[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                       // 0 ~ 1の範囲外をリピート
    current_[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    current_[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    current_[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;                     // 比較しない
    current_[0].MaxLOD = D3D12_FLOAT32_MAX;                                       // ありったけのーを使う
    current_[0].ShaderRegister = 0;                                               // レジスタ番号0を使用する
    current_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderを使う
}