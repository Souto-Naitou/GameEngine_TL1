#include "RootSignature.h"
#include <Logger.h>
#include <cassert>

void RootSignature::Configurator::Initialize(ID3D12Device* _device)
{
    pRootParametersCfg_ = std::make_unique<RootParameters::Configurator>();
    pStaticSamplerCfg_ = std::make_unique<StaticSamplers::Configurator>();

    pRootParametersCfg_->Initialize();
    pStaticSamplerCfg_->Initialize();

    current_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    current_.pParameters = pRootParametersCfg_->Get()->data();           // ルートパラメータ配列へのポインタ
    current_.NumParameters = pRootParametersCfg_->Get()->size();      // 配列の長さ
    current_.pStaticSamplers = pStaticSamplerCfg_->Get()->data();
    current_.NumStaticSamplers = pStaticSamplerCfg_->Get()->size();

    /// シリアライズしてバイナリにする
    HRESULT hr_ = D3D12SerializeRootSignature(&current_,
        D3D_ROOT_SIGNATURE_VERSION_1, &pSignatureBlob_, &pErrorBlob_);
    if (FAILED(hr_))
    {
        Log(reinterpret_cast<char*>(pErrorBlob_->GetBufferPointer()));
        assert(false);
    }

    /// バイナリをもとに生成
    hr_ = _device->CreateRootSignature(0, pSignatureBlob_->GetBufferPointer(),
        pSignatureBlob_->GetBufferSize(), IID_PPV_ARGS(&pRootSignature_));
    assert(SUCCEEDED(hr_));

    return;
}
