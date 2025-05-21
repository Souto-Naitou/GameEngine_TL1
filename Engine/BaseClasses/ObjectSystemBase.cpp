#include "ObjectSystemBase.h"

void ObjectSystemBase::Initialize()
{
    pDx12_ = DirectX12::GetInstance();
    CreateCommandList();
}

void ObjectSystemBase::PostDraw()
{
    commandAllocator_->Reset();
    commandList_->Reset(commandAllocator_.Get(), nullptr);
}

void ObjectSystemBase::CreateCommandList()
{
    auto device = pDx12_->GetDevice();
    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator_.GetAddressOf()));
    device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(commandList_.GetAddressOf()));
}
