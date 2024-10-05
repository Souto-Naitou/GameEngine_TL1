#pragma once

#include <BaseConfigurator.h>
#include <DescriptorRange.h>

#include <d3d12.h>
#include <array>
#include <memory>

namespace RootParameters
{
    class Configurator : public BaseConfigurator<std::array<D3D12_ROOT_PARAMETER, 4>>
    {
    public:
        void Initialize();

    private:
        std::unique_ptr<DescriptorRange::Configurator> pDescriptorRangeCfg_ = nullptr;
    };
}