#pragma once

#include "DirectX12.h"
#include <wrl.h>

class SpriteSystem
{
public:
    SpriteSystem();
    ~SpriteSystem();

    void Initialize();
    void Update();
    void PresentDraw();

    DirectX12* GetDx12() { return pDx12_; }

private:
    DirectX12* pDx12_ = nullptr;
    ID3D12RootSignature* rootSignature_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

private:
    void CreateRootSignature();
    void CreatePipelineState();

};