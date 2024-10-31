#include <Windows.h>
#include "Core/DirectX12/DirectX12.h"
#include <Win32Application.h>
#include "Features/Sprite/SpriteSystem.h"

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Win32Application* pWin32App = Win32Application::GetInstance();
    DirectX12* pDirectX = DirectX12::GetInstance();
    SpriteSystem* pSpriteSystem = new SpriteSystem();

    /// ウィンドウの初期化
    pWin32App->Initialize();
    pWin32App->ShowWnd();

    /// DirectX12の初期化
    pDirectX->Initialize();

    /// スプライトシステムの初期化
    pSpriteSystem->Initialize();


    while (pWin32App->GetMsg() != WM_QUIT)
    {
        pDirectX->PresentDraw();



        pDirectX->PostDraw();
    }

    pWin32App->Finalize();


    delete pSpriteSystem;

    return 0;
}