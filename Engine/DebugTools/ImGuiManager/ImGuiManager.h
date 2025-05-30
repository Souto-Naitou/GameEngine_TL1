#pragma once

#ifdef _DEBUG

#include <d3d12.h>
#include <cstdint>

#include <imgui.h>

class DirectX12;

class ImGuiManager
{
public:
    // Common functions
    void Initialize(DirectX12* _pDx12);
    void BeginFrame();
    void Render();                          // 描画
    void EndFrame();                        // フレーム終了
    void RenderMultiViewport();             // Draw multiple viewports. Call this after executing the command list, before presenting the swapchain.
    void Finalize();
    void Resize();

    // Switch options
    void EnableDocking();
    void EnableMultiViewport();

private:
    ImGuiIO* io_ = nullptr;
    bool isChangedFont_ = false;
    ID3D12DescriptorHeap* srvDescHeap_ = nullptr;
    uint32_t srvIndex_ = 0u;
};

#endif // _DEBUG