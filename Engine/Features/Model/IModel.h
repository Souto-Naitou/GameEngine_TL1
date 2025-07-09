#pragma once
#include <d3d12.h>
#include <memory>

class IModel
{
public:
    virtual ~IModel() = default;
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void ChangeTexture(D3D12_GPU_DESCRIPTOR_HANDLE _texSrvHnd) = 0;
    virtual void CreateGPUResource() = 0;
    virtual void SetCommandList(ID3D12GraphicsCommandList* _cl) = 0;
    virtual bool IsEndLoading() const = 0;
    virtual void Clone(IModel* _src) = 0;
    
    virtual D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const = 0;
    virtual size_t GetVertexCount() const = 0;
    virtual D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandle() const = 0;
};