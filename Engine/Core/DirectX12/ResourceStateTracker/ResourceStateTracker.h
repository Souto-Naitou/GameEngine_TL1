#pragma once
#include <wrl/client.h>
#include <d3d12.h>

struct ResourceStateTracker
{
    Microsoft::WRL::ComPtr<ID3D12Resource> resource;
    D3D12_RESOURCE_STATES state;

    void Reset();
};