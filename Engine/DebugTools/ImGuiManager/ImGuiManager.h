#pragma once

#ifdef _DEBUG

#include <d3d12.h>
#include <cstdint>

#include <imgui.h>
#include <string>
#include <vector>

class DirectX12;

class ImGuiManager
{
public:
    // Common functions
    void Initialize(DirectX12* _pDx12);
    void BeginFrame();
    void Render();                          // 描画
    void EndFrame();                        // フレーム終了
    void Finalize();
    void Resize();

    // Switch options
    void EnableDocking();
    void EnableMultiViewport();
    
    // Style changes
    void StyleOriginal();
    void StylePhotoshop();
    void StyleMaterialFlat();
    void StyleFutureDark();
    void StyleComfortableDarkCyan();

private:
    // For debug
    void DebugWindow();

    // Internal functions
    void InitializeStyleNameArray();

    ImGuiIO*                io_             = nullptr;
    bool                    isChangedFont_  = false;
    uint32_t                srvIndex_       = 0u;
    ID3D12DescriptorHeap*   srvDescHeap_    = nullptr;

    // For debug window
    std::vector<std::string>    styleNameArray_     = {};
    size_t                      idx_currentStyle_   = 0u;
};

#endif // _DEBUG