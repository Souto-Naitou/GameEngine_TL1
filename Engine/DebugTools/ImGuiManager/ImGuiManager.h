#pragma once

#include <d3d12.h>
#include <cstdint>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

class DirectX12;

class ImGuiManager
{
public:
    void Initialize(DirectX12* _pDx12);
    void BeginFrame();
    void Render();
    void EndFrame();
    void Finalize();
    void Resize();

private:
    bool isChangedFont_ = false;
    ID3D12DescriptorHeap* srvDescHeap_ = nullptr;
    uint32_t srvIndex_ = 0u;
};