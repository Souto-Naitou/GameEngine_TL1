#pragma once

#include <future>
#include <d3d12.h>
#include <wrl/client.h>
#include <Core/DirectX12/DirectX12.h>

class ObjectSystemBase
{
public:
    virtual         ~ObjectSystemBase() = default;

    virtual void    Initialize();
    virtual void    DrawCall() = 0;
    virtual void    Sync() = 0;


public: /// Getter
    DirectX12* GetDx12() { return pDx12_; }
    ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }


protected:
    std::future<void> worker_;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;


protected:
    void CreateCommandList();


protected:
    DirectX12*  pDx12_ = nullptr;

};