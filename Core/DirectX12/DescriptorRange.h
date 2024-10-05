#pragma once

#include <BaseConfigurator.h>

#include <d3d12.h>
#include <array>

namespace DescriptorRange
{

    class Configurator : public BaseConfigurator <std::array<D3D12_DESCRIPTOR_RANGE, 1>>
    {
    public:
        void Initialize();
    };

}