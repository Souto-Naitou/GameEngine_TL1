#pragma once

#include <future>
#include <d3d12.h>
#include <wrl/client.h>

// Forward declarations
class DirectX12;
class GameEye;

class ObjectSystemBase
{
public:
    // Virtual functions
    virtual         ~ObjectSystemBase() = default;      // dtor
    virtual void    Initialize();                       // Create the command list
    virtual void    DrawCall() = 0;                     // Draw call (e.g. processing a batch of draw commands)
    virtual void    Sync() = 0;                         // Sync with main thread
    virtual void    PostDraw();                         // Reset the command allocator and command list
    
    // Getters
    DirectX12*                      GetDx12();          // Get the instance of the DirectX12 class
    ID3D12GraphicsCommandList*      GetCommandList();   // Get the command list
    GameEye**                       GetGlobalEye();     // Get the GameEye for global use

    // Setters
    void    SetGlobalEye(GameEye* _pGameEye);                       // Set the GameEye for global use
    void    SetRTVHandle(D3D12_CPU_DESCRIPTOR_HANDLE* _rtvHandle);  // Set the render target view handle

protected:
    template <typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    // Threads and DirectX objects
    std::future<void>                   worker_             = {};       // Worker thread
    ComPtr<ID3D12CommandAllocator>      commandAllocator_   = nullptr;
    ComPtr<ID3D12GraphicsCommandList>   commandList_        = nullptr;

    // Internal functions
    void CreateCommandList();

    // Pointers
    DirectX12*                          pDx12_              = nullptr;
    GameEye*                            pGlobalEye_         = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE*        rtvHandle_          = nullptr;
};