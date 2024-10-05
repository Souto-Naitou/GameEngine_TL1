#pragma once

#include <BaseConfigurator.h>
#include <d3d12.h>

namespace InputLayout
{


    class Configurator : public BaseConfigurator<D3D12_INPUT_LAYOUT_DESC>
    {
    public:
        void Initialize();

    private:
        D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3] = {};
    };
}