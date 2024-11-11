#include <Windows.h>
#include "Core/DirectX12/DirectX12.h"
#include <Win32Application.h>
#include "Features/Sprite/SpriteSystem.h"
#include "Features/Sprite/Sprite.h"
#include "Core/DirectX12/TextureManager.h"

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Win32Application* pWin32App = Win32Application::GetInstance();
    DirectX12* pDirectX = DirectX12::GetInstance();
    SpriteSystem* pSpriteSystem = new SpriteSystem();
    Sprite* pSprite = new Sprite();
    Sprite* pSpriteMB = new Sprite();
    TextureManager::GetInstance()->Initialize();

    /// ウィンドウの初期化
    pWin32App->Initialize();
    pWin32App->ShowWnd();

    /// DirectX12の初期化
    pDirectX->Initialize();

    /// スプライトシステムの初期化
    pSpriteSystem->Initialize();
    Vector3 rotate = { 0.0f, 0.0f, 0.0f };
    Vector3 transform = { 0.0f, 0.0f, 0.0f };
    pSprite->Initialize(pSpriteSystem, "uvChecker.png", {128,128}, rotate, transform);
    pSpriteMB->Initialize(pSpriteSystem, "monsterBall.png", {128,64}, rotate, transform);

    while (pWin32App->GetMsg() != WM_QUIT)
    {
        /// スプライトを移動させる

        pSprite->SetPosition({0.f, 0.f});
        pSpriteMB->SetPosition({0.f, 360.f});

        pSprite->Update();
        pSpriteMB->Update();

        pDirectX->PresentDraw();

        pSpriteSystem->PresentDraw();
        pSprite->Draw();
        pSpriteMB->Draw();

        pDirectX->PostDraw();
    }

    pWin32App->Finalize();


    delete pSprite;
    delete pSpriteMB;
    delete pSpriteSystem;

    return 0;
}