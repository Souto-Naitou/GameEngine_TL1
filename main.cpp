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
#include <ModelManager.h>

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    DirectX12* pDirectX = DirectX12::GetInstance();

    Win32Application* pWin32App = Win32Application::GetInstance();
    TextureManager::GetInstance()->Initialize();

    ModelManager* modelManager = ModelManager::GetInstance();
    modelManager->Initialize();

    /// 基盤の初期化
    ImGuiManager* pImGuiManager = new ImGuiManager();
    DebugManager* pDebugManager = DebugManager::GetInstance();
    SpriteSystem* pSpriteSystem = new SpriteSystem();
    Object3dSystem* pObject3dSystem = new Object3dSystem();

    /// ゲーム内オブジェクトの宣言
    Object3d* pObject3d = new Object3d();
    Model* planeObj = nullptr;

    /// ウィンドウの初期化
    pWin32App->Initialize();
    pWin32App->ShowWnd();

    /// DirectX12の初期化
    pDirectX->Initialize();

    /// スプライト基盤の初期化
    pSpriteSystem->Initialize();

    /// 3Dオブジェクト基盤の初期化
    pObject3dSystem->Initialize();

    /// ゲーム内オブジェクトの初期化
    pObject3d->Initialize(pObject3dSystem);
    pObject3d->SetScale({ -1.0f, 1.0f, 1.0f });

    modelManager->LoadModel("plane.obj");
    planeObj = modelManager->FindModel("plane.obj");
    pObject3d->SetModel(planeObj);

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