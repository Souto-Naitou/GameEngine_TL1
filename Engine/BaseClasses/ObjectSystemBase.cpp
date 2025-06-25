#include "ObjectSystemBase.h"

void ObjectSystemBase::Initialize()
{
    CreateCommandList();
}

void ObjectSystemBase::PostDraw()
{
    commandAllocator_->Reset();
    commandList_->Reset(commandAllocator_.Get(), nullptr);
}

ID3D12GraphicsCommandList* ObjectSystemBase::GetCommandList()
{
    return commandList_.Get();
}

GameEye** ObjectSystemBase::GetGlobalEye()
{
    return &pGlobalEye_;
}

void ObjectSystemBase::SetGlobalEye(GameEye* _pGameEye)
{
    pGlobalEye_ = _pGameEye;
}

void ObjectSystemBase::SetRTVHandle(D3D12_CPU_DESCRIPTOR_HANDLE* _handle)
{
    rtvHandle_ = _handle;
}

void ObjectSystemBase::CreateCommandList()
{
    auto device = pDx12_->GetDevice();
    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator_.GetAddressOf()));
    device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(commandList_.GetAddressOf()));
}
