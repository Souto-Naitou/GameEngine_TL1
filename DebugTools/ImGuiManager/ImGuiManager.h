#pragma once

#include <d3d12.h>
#include <imgui.h>

class DirectX12;

class ImGuiManager
{
public:
    void Initialize(DirectX12* _pDx12);
    void BeginFrame();
    void Render();
    void EndFrame();
    void Finalize();

private:
    ID3D12DescriptorHeap* srvDescHeap_ = nullptr;
    bool isChangedFont_ = false;
};