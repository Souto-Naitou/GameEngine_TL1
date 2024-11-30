#pragma once

#include <d3d12.h>

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

private:
    ID3D12DescriptorHeap* srvDescHeap_ = nullptr;
    bool isChangedFont_ = false;
};