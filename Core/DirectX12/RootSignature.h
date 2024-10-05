#pragma once

#include <BaseConfigurator.h>
#include <RootParameters.h>
#include <StaticSamplers.h>

#include <d3d12.h>
#include <memory>
#include <wrl.h>

namespace RootSignature
{
    class Configurator : public BaseConfigurator<D3D12_ROOT_SIGNATURE_DESC>
    {
    public:
        void Initialize(ID3D12Device* _device);

        inline ID3D12RootSignature* Get() { return pRootSignature_.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3DBlob>                pSignatureBlob_     = nullptr;
        Microsoft::WRL::ComPtr<ID3DBlob>                pErrorBlob_         = nullptr;
        Microsoft::WRL::ComPtr<ID3D12RootSignature>     pRootSignature_     = nullptr;
        std::unique_ptr<RootParameters::Configurator>   pRootParametersCfg_ = nullptr;
        std::unique_ptr<StaticSamplers::Configurator>   pStaticSamplerCfg_  = nullptr;
    };
}