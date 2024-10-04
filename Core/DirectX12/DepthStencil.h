#pragma once

#include <d3d12.h>

#include <BaseConfigurator.h>

namespace DepthStencil
{

    class Configurator : public BaseConfigurator<D3D12_DEPTH_STENCIL_DESC>
    {
    public:
        void Initialize();
    };

}