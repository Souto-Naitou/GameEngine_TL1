#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include "wrl.h"

void CreateDevice(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<IDXGIAdapter4>& _adapter);
void PauseError(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<ID3D12InfoQueue>& _infoQ);