#include <Windows.h>
#include "Core/DirectX12/DirectX12.h"
#include <Win32Application.h>
#include "Features/Sprite/SpriteSystem.h"
#include "Features/Sprite/Sprite.h"

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Win32Application* pWin32App = Win32Application::GetInstance();
    DirectX12* pDirectX = DirectX12::GetInstance();
    SpriteSystem* pSpriteSystem = new SpriteSystem();
    Sprite* pSprite = new Sprite();

    /// ウィンドウの初期化
    pWin32App->Initialize();
    pWin32App->ShowWnd();

    /// DirectX12の初期化
    pDirectX->Initialize();

    /// スプライトシステムの初期化
    pSpriteSystem->Initialize();
    Vector3 scale = { 1.0f, 1.0f, 1.0f };
    Vector3 rotate = { 0.0f, 0.0f, 0.0f };
    Vector3 transform = { 0.0f, 0.0f, 0.0f };
    pSprite->Initialize(pSpriteSystem, "uvChecker.png", scale, rotate, transform);

    while (pWin32App->GetMsg() != WM_QUIT)
    {
        pSprite->Update();

        pDirectX->PresentDraw();

        pSpriteSystem->PresentDraw();
        pSprite->Draw();

        pDirectX->PostDraw();
    }

    pWin32App->Finalize();


    delete pSprite;
    delete pSpriteSystem;

    return 0;
}