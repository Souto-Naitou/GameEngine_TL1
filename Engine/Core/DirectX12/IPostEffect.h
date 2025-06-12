#pragma once
#include <d3d12.h>
#include <string>


class IPostEffect
{
    friend class PostEffect;

public:
    IPostEffect() = default;
    virtual ~IPostEffect() = default;

    // Initialize the post effect
    virtual void    Initialize() = 0;
    // Release resources
    virtual void    Release() = 0;

    virtual void    Enable(bool _flag) = 0;
    virtual bool    Enabled() const = 0;

private:
    // Setter
    virtual void    SetInputTextureHandle(D3D12_GPU_DESCRIPTOR_HANDLE handle) = 0;
    // Getter
    virtual D3D12_GPU_DESCRIPTOR_HANDLE GetOutputTextureHandle() const = 0;
    virtual const std::string& GetName() const = 0;

    // Draw setting
    virtual void    Setting() = 0;
    // Apply the post effect
    virtual void    Apply() = 0;
    // Change the state of the render texture to shader resource state
    virtual void    ToShaderResourceState() = 0;
    // On resize event
    virtual void    OnResizeBefore() = 0;
    virtual void    OnResizedBuffers() = 0;
    virtual void    DebugOverlay() = 0;
};