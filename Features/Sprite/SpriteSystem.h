#pragma once

#include "DirectX12.h"

class SpriteSystem
{
public:
    SpriteSystem();
    ~SpriteSystem();

    void Initialize(DirectX12* _dx12);
    void Update();

    DirectX12* GetDx12() { return pDx12_; }

private:
    DirectX12* pDx12_ = nullptr;

private:
    void CreateRootSignature();
    void CreatePipelineState();

};