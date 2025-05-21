#include "ImGuiManager.h"

#ifdef _DEBUG

#include <Core/Win32/WinSystem.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/SRVManager.h>
#include <DebugTools/DebugManager/DebugManager.h>

#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

void ImGuiManager::Initialize(DirectX12* _pDx12)
{
    SRVManager* srvManager = SRVManager::GetInstance();
    srvIndex_ = srvManager->Allocate();

    WinSystem* pWin32App = WinSystem::GetInstance();
    ID3D12Device* device = _pDx12->GetDevice();
    const DXGI_SWAP_CHAIN_DESC1& swapChainDesc = _pDx12->GetSwapChainDesc();
    srvDescHeap_ = srvManager->GetDescriptorHeap();
    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = srvManager->GetCPUDescriptorHandle(srvIndex_);
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = srvManager->GetGPUDescriptorHandle(srvIndex_);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplDX12_Init(
        device,
        swapChainDesc.BufferCount,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        srvDescHeap_,
        srvDescHeap_->GetCPUDescriptorHandleForHeapStart(),
        srvDescHeap_->GetGPUDescriptorHandleForHeapStart()
    );
    ImGui_ImplWin32_Init(pWin32App->GetHwnd());
}

void ImGuiManager::Finalize()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::Resize()
{
    auto& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(WinSystem::clientWidth);
    io.DisplaySize.y = static_cast<float>(WinSystem::clientHeight);

    float scaleX = static_cast<float>(WinSystem::clientWidth) / static_cast<float>(WinSystem::preClientWidth);
    float scaleY = static_cast<float>(WinSystem::clientHeight) / static_cast<float>(WinSystem::preClientHeight);

    io.DisplayFramebufferScale = ImVec2(scaleX, scaleY);

    auto* drawData = ImGui::GetDrawData();
    if(drawData)
    {
        drawData->DisplayPos = ImVec2(0.0f, 0.0f);
        drawData->DisplaySize = ImVec2(io.DisplaySize.x, io.DisplaySize.y);
    }
}

void ImGuiManager::BeginFrame()
{
    if(!isChangedFont_)
    {
        DebugManager* debugManager = DebugManager::GetInstance();
        debugManager->ChangeFont();
        debugManager->DefaultStyle();
        debugManager->EnableDocking();

        isChangedFont_ = true;
    }

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::Render()
{
    ImGui::EndFrame();
    ImGui::Render();
}

void ImGuiManager::EndFrame()
{
    ID3D12GraphicsCommandList* commandList = DirectX12::GetInstance()->GetCommandListsLast();

    //ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescHeap_ };
    //commandList->SetDescriptorHeaps(1, descriptorHeaps);

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

#endif // _DEBUG