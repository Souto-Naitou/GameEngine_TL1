#pragma once

#include <Core/DirectX12/DirectX12.h>
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
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

private:
    void CreateRootSignature();
    void CreatePipelineState();
    static constexpr wchar_t kVertexShaderPath[] = L"EngineResources/Shaders/Sprite.VS.hlsl";
    static constexpr wchar_t kPixelShaderPath[] = L"EngineResources/Shaders/Sprite.PS.hlsl";
};