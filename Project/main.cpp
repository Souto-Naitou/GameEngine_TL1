#include <Common\define.h>
#include <Windows.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/Win32/Win32Application.h>
#include <Features/Sprite/SpriteSystem.h>
#include <Features/Sprite/Sprite.h>
#include <Features/Object3d/Object3dSystem.h>
#include <Features/Object3d/Object3d.h>
#include <Core/DirectX12/TextureManager.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <DebugTools/ImGuiManager/ImGuiManager.h>
#include <Features/Model/ModelManager.h>
#include <Features/Model/Model.h>
#include <Features/Input/Input.h>

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    DirectX12* pDirectX = DirectX12::GetInstance();
    DebugManager* pDebugManager = DebugManager::GetInstance();

    Win32Application* pWin32App = Win32Application::GetInstance();
    TextureManager::GetInstance()->Initialize();

    ModelManager* modelManager_ = ModelManager::GetInstance();
    modelManager_->Initialize();
    modelManager_->AddAutoLoadPath("resources/models");

    /// 基盤の初期化
    ImGuiManager* pImGuiManager = new ImGuiManager();
    SpriteSystem* pSpriteSystem = new SpriteSystem();
    Object3dSystem* pObject3dSystem = new Object3dSystem();

    /// ゲーム内オブジェクトの宣言
    Object3d* pObject3d = new Object3d();
    Model* planeObj = new Model();

    /// ウィンドウの初期化
    pWin32App->Initialize();
    pWin32App->ShowWnd();

    /// 入力の初期化
    Input* pInput = new Input();
    pInput->Initialize(GetModuleHandleA(nullptr), pWin32App->GetHwnd());


    /// DirectX12の初期化
    pDirectX->Initialize();

    /// スプライト基盤の初期化
    pSpriteSystem->Initialize();

    /// 3Dオブジェクト基盤の初期化
    pObject3dSystem->Initialize();

    /// ゲーム内オブジェクトの初期化
    modelManager_->LoadAllModel();
    pObject3d->Initialize("suzanne.obj");
    pObject3d->SetScale({ -1.0f, 1.0f, 1.0f });

    pImGuiManager->Initialize(pDirectX);


    while (pWin32App->GetMsg() != WM_QUIT)
    {
        /// マネージャ更新
        modelManager_->Update();

        pInput->Update();


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

    delete planeObj;
    delete pObject3d;
    delete pObject3dSystem;
    delete pSpriteSystem;
    delete pImGuiManager;

    return 0;
}