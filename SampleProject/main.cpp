#include <Common/define.h>
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
#include <Features/GameEye/GameEye.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/Particle/Particle.h>
#include <Core/DirectX12/SRVManager.h>

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    DirectX12* pDirectX = DirectX12::GetInstance();
    DebugManager* pDebugManager = DebugManager::GetInstance();

    Win32Application* pWin32App = Win32Application::GetInstance();

    ModelManager* modelManager_ = ModelManager::GetInstance();
    modelManager_->Initialize();
    modelManager_->AddAutoLoadPath("resources/models");

    /// 基盤の初期化
    SRVManager* pSRVManager = SRVManager::GetInstance();
    ImGuiManager* pImGuiManager = new ImGuiManager();
    SpriteSystem* pSpriteSystem = new SpriteSystem();
    Object3dSystem* pObject3dSystem = new Object3dSystem();
    ParticleSystem* pParticleSystem = new ParticleSystem();
    TextureManager::GetInstance()->Initialize(pSRVManager);

    /// ゲーム内オブジェクトの宣言
    Object3d* pObject3d = new Object3d();
    GameEye* pGameEye = new GameEye();
    Sprite* pSpriteMB = new Sprite();
    Sprite* pSpriteUVC = new Sprite();
    Particle* pParticle = new Particle();
    pGameEye->SetRotate({ 0.0f, -0.4f, 0.0f });
    pGameEye->SetTranslate({ 5.0f, 0.0f, -10.0f });
    pGameEye->SetName("MainCamera");

    /// ウィンドウの初期化
    pWin32App->Initialize();
    pWin32App->ShowWnd();

    /// 入力の初期化
    Input* pInput = new Input();
    pInput->Initialize(GetModuleHandleA(nullptr), pWin32App->GetHwnd());


    /// DirectX12の初期化
    pDirectX->Initialize();

    /// SRVManagerの初期化
    pSRVManager->Initialize(pDirectX);

    /// スプライト基盤の初期化
    pSpriteSystem->Initialize();

    /// 3Dオブジェクト基盤の初期化
    pObject3dSystem->Initialize();
    pObject3dSystem->SetDefaultGameEye(pGameEye);
    pParticleSystem->Initialize();
    pParticleSystem->SetDefaultGameEye(pGameEye);

    /// ゲーム内オブジェクトの初期化
    modelManager_->LoadAllModel();
    pObject3d->Initialize(pObject3dSystem, "suzanne.obj");
    pObject3d->SetScale({ -1.0f, 1.0f, 1.0f });
    pObject3d->SetName("Suzanne");

    pSpriteMB->Initialize(pSpriteSystem, "MonsterBall.png");
    pSpriteMB->SetName("MonsterBall");
    pSpriteMB->SetSize({ 120,60 });
    pSpriteMB->SetPosition({ 40,60 });
    pSpriteUVC->Initialize(pSpriteSystem, "uvChecker.png");
    pSpriteUVC->SetName("uvChecker");
    pSpriteUVC->SetSize({ 120,120 });
    pSpriteUVC->SetPosition({ 180, 60 });

    pParticle->Initialize(pParticleSystem, "plane.obj");

    pImGuiManager->Initialize(pDirectX);


    while (true)
    {
        UINT msg = pWin32App->GetMsg();
        if (msg == WM_QUIT)
        {
            break;
        }

        /// マネージャ更新
        modelManager_->Update();

        pImGuiManager->BeginFrame();
        pDebugManager->DrawUI();
        pImGuiManager->Render();

        /// 更新処理
        pInput->Update();
        pGameEye->Update();
        pObject3d->Update();
        pSpriteMB->Update();
        pSpriteUVC->Update();
        pParticle->Update();

        /// 描画処理
        pDirectX->PresentDraw();
        pSRVManager->PresentDraw();
        pSpriteSystem->PresentDraw();
        //pSpriteMB->Draw();
        //pSpriteUVC->Draw();

        pObject3dSystem->PresentDraw();
        //pObject3d->Draw();

        pParticleSystem->PresentDraw();
        pParticle->Draw();


        pImGuiManager->EndFrame();
        pDirectX->PostDraw();

    }

    /// 終了処理
    pSpriteUVC->Finalize();
    pSpriteMB->Finalize();
    pObject3d->Finalize();
    pImGuiManager->Finalize();
    pWin32App->Finalize();

    /// ゲーム内オブジェクトの解放
    delete pParticle;
    delete pSpriteUVC;
    delete pSpriteMB;
    delete pGameEye;
    delete pObject3d;
    delete pParticleSystem;
    delete pObject3dSystem;
    delete pSpriteSystem;
    delete pImGuiManager;

    return 0;
}