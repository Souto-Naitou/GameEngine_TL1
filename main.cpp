#include <define.h>
#include <Windows.h>
#include "Core/DirectX12/DirectX12.h"
#include <Win32Application.h>
#include "Features/Sprite/SpriteSystem.h"
#include "Features/Sprite/Sprite.h"
#include "Features/Object3d/Object3dSystem.h"
#include "Features/Object3d/Object3d.h"
#include "Core/DirectX12/TextureManager.h"
#include <DebugTools/DebugManager/DebugManager.h>
#include <DebugTools/ImGuiManager/ImGuiManager.h>

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    DirectX12* pDirectX = DirectX12::GetInstance();
    Win32Application* pWin32App = Win32Application::GetInstance();
    TextureManager::GetInstance()->Initialize();

    ImGuiManager* pImGuiManager = new ImGuiManager();
    DebugManager* pDebugManager = DebugManager::GetInstance();

    SpriteSystem* pSpriteSystem = new SpriteSystem();

    Object3dSystem* pObject3dSystem = new Object3dSystem();
    Object3d* pObject3d = new Object3d();

    /// ウィンドウの初期化
    pWin32App->Initialize();
    pWin32App->ShowWnd();

    /// DirectX12の初期化
    pDirectX->Initialize();

    /// スプライト関連の初期化
    pSpriteSystem->Initialize();
    Vector3 rotate = { 0.0f, 0.0f, 0.0f };
    Vector3 transform = { 0.0f, 0.0f, 0.0f };

    /// 3Dオブジェクト関連の初期化
    pObject3dSystem->Initialize();
    pObject3d->Initialize(pObject3dSystem, "plane.obj");
    pObject3d->SetScale({ -1.0f, 1.0f, 1.0f });

    pImGuiManager->Initialize(pDirectX);


    while (pWin32App->GetMsg() != WM_QUIT)
    {
        pImGuiManager->BeginFrame();

        pDebugManager->DrawUI();

        pImGuiManager->Render();

        pObject3d->Update();

        pDirectX->PresentDraw();

        pSpriteSystem->PresentDraw();

        pObject3dSystem->PresentDraw();
        pObject3d->Draw();

        pImGuiManager->EndFrame();
        pDirectX->PostDraw();

    }

    pObject3d->Finalize();
    pImGuiManager->Finalize();
    pWin32App->Finalize();

    delete pObject3d;
    delete pObject3dSystem;
    delete pSpriteSystem;
    delete pImGuiManager;

    return 0;
}