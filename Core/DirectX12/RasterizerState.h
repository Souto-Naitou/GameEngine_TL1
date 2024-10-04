#pragma once

#include <d3d12.h>

#include <BaseConfigurator.h>

namespace RasterizerState
{

    class Configurator : public BaseConfigurator<D3D12_RASTERIZER_DESC>
    {
    public:
        void Initialize();
    };
}