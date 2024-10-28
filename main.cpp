#include <Windows.h>
#include "Core/DirectX12/DirectX12.h"
#include <Win32Application.h>

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Win32Application* pWin32App = Win32Application::GetInstance();
    DirectX12* pDirectX = DirectX12::GetInstance();

    pWin32App->Initialize();
    pWin32App->ShowWnd();

    pDirectX->Initialize();

    while (pWin32App->GetMsg() != WM_QUIT)
    {
        pDirectX->PresentDraw();
        pDirectX->PostDraw();
    }

    pWin32App->Finalize();

    return 0;
}