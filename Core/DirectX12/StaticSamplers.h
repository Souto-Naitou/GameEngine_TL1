#pragma once

#include <BaseConfigurator.h>
#include <d3d12.h>
#include <array>

namespace StaticSamplers
{
    

    class Configurator : public BaseConfigurator <std::array<D3D12_STATIC_SAMPLER_DESC, 1>>
    {
    public:
        void Initialize();
    };
}