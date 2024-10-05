#include <RootParameters.h>

void RootParameters::Configurator::Initialize()
{
    /// 
    pDescriptorRangeCfg_ = std::make_unique<DescriptorRange::Configurator>();
    pDescriptorRangeCfg_->Initialize();

    /// RootParameter作成。複数設定できるので配列。
    current_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使う
    current_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使う
    current_[0].Descriptor.ShaderRegister = 0;                                    // レジスタ番号０とバインド

    current_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使う
    current_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;                // VertexShaderで使う
    current_[1].Descriptor.ShaderRegister = 0;                                    // レジスタ番号０とバインド

    current_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;       // DescriptorTableを使う
    current_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使う
    current_[2].DescriptorTable.pDescriptorRanges = &(*pDescriptorRangeCfg_->Get())[0];       // Tableの中身の配列を指定
    current_[2].DescriptorTable.NumDescriptorRanges = pDescriptorRangeCfg_->Get()->size();  // Tableで利用する数

    current_[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                    // CBVを使用する
    current_[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                 // PixelShaderで使用する
    current_[3].Descriptor.ShaderRegister = 1;                                    // レジスタ番号1を使用する
}