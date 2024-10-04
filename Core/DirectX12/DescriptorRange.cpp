#include <DescriptorRange.h>

void DescriptorRange::Configurator::Initialize()
{
    /// ディスクリプタテーブルのディスクリプタレンジを設定する (SRVを通じてシェーダにリソースへのアクセスを提供・t0)
    current_[0].BaseShaderRegister = 0; // 0から始まる
    current_[0].NumDescriptors = 1; // 数は1つ
    current_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
    current_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
}