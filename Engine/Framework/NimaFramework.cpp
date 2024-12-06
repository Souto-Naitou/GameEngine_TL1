#include "NimaFramework.h"

void NimaFramework::Run()
{
    /// 初期化
    Initialize();

    while (true)
    {
        /// 更新
        Update();

        /// 終了判定
        if (IsExitProgram())
        {
            break;
        }

        /// 描画
        Draw();
    }

    Finalize();

    return;
}

void NimaFramework::Initialize()
{
    /// システムクラスの初期化
    pDirectX_ = DirectX12::GetInstance();
    pDebugManager_ = DebugManager::GetInstance();
    pWin32App_ = Win32Application::GetInstance();
    pModelManager_ = ModelManager::GetInstance();
    pTextureManager_ = TextureManager::GetInstance();
    pSRVManager_ = SRVManager::GetInstance();

    pAudio_ = new Audio();
    pInput_ = new Input();

    pImGuiManager_ = new ImGuiManager();
    pSpriteSystem_ = new SpriteSystem();
    pObject3dSystem_ = new Object3dSystem();
    pParticleSystem_ = new ParticleSystem();

    /// ウィンドウの初期化
    pWin32App_->Initialize();
    pWin32App_->ShowWnd();

    /// DirectX12の初期化
    pDirectX_->Initialize();

    /// SRVManagerの初期化
    pSRVManager_->Initialize(pDirectX_);

    /// ImGui基盤の初期化
    pImGuiManager_->Initialize(pDirectX_);

    /// テクスチャマネージャの初期化
    pTextureManager_->Initialize(pSRVManager_);

    /// モデルマネージャの初期化
    pModelManager_->Initialize();

    /// スプライト基盤の初期化
    pSpriteSystem_->Initialize();

    /// 3Dオブジェクト基盤の初期化
    pObject3dSystem_->Initialize();

    /// オーディオの初期化
    pAudio_->Initialize();

    /// 入力の初期化
    pInput_->Initialize(GetModuleHandleA(nullptr), pWin32App_->GetHwnd());
}

void NimaFramework::Finalize()
{
    delete pParticleSystem_;
    delete pObject3dSystem_;
    delete pSpriteSystem_;
    delete pImGuiManager_;
}

void NimaFramework::Update()
{
    UINT msg = pWin32App_->GetMsg();
    if (msg == WM_QUIT)
    {
        isExitProgram_ = true;
        return;
    }

    /// マネージャ更新
    pModelManager_->Update();
    pImGuiManager_->BeginFrame();
    pDebugManager_->DrawUI();
    pImGuiManager_->Render();
}
