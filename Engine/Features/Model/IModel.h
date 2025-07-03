#pragma once
#include <d3d12.h>

class IModel
{
public:
    virtual ~IModel() = default;
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void ChangeTexture(D3D12_GPU_DESCRIPTOR_HANDLE _texSrvHnd) = 0;
};