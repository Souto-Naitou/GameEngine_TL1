#include "ImGuiManager.h"

#include <Win32Application.h>
#include <DirectX12.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include <DebugTools/DebugManager/DebugManager.h>

void ImGuiManager::Initialize(DirectX12* _pDx12)
{
    Win32Application* pWin32App = Win32Application::GetInstance();
    ID3D12Device* device = _pDx12->GetDevice();
    const DXGI_SWAP_CHAIN_DESC1& swapChainDesc = _pDx12->GetSwapChainDesc();
    srvDescHeap_ = _pDx12->GetSRVDescriptorHeap();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(pWin32App->GetHwnd());
    ImGui_ImplDX12_Init(
        device,
        swapChainDesc.BufferCount,
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        srvDescHeap_,
        srvDescHeap_->GetCPUDescriptorHandleForHeapStart(),
        srvDescHeap_->GetGPUDescriptorHandleForHeapStart()
    );
}

void ImGuiManager::Finalize()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::BeginFrame()
{
    if (!isChangedFont_)
    {
        DebugManager::GetInstance()->ChangeFont();
        DebugManager::GetInstance()->RoundedVisualStudioStyle();
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
    ID3D12GraphicsCommandList* commandList = DirectX12::GetInstance()->GetCommandList();

    ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescHeap_ };
    commandList->SetDescriptorHeaps(1, descriptorHeaps);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}
