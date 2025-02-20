#include "NimaFramework.h"
#include <clocale>

void NimaFramework::Run()
{
    setlocale(LC_ALL, "ja_JP.Utf-8");

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
    pLogger_ = Logger::GetInstance();
    pDirectX_ = DirectX12::GetInstance();
    pDebugManager_ = DebugManager::GetInstance();
    pWinSystem_ = WinSystem::GetInstance();
    pModelManager_ = ModelManager::GetInstance();
    pSpriteSystem_ = SpriteSystem::GetInstance();
    pObject3dSystem_ = Object3dSystem::GetInstance();
    pParticleSystem_ = ParticleSystem::GetInstance();
    pTextureManager_ = TextureManager::GetInstance();
    pSRVManager_ = SRVManager::GetInstance();
    pSceneManager_ = SceneManager::GetInstance();
    pParticleManager_ = ParticleManager::GetInstance();
    pLineSystem_ = LineSystem::GetInstance();
    pInput_ = Input::GetInstance();
    pRandomGenerator_ = RandomGenerator::GetInstance();
    pTextSystem_ = TextSystem::GetInstance();
    pAudioManager_ = AudioManager::GetInstance();
    pImGuiManager_ = std::make_unique<ImGuiManager>();

    /// ロガーの初期化
    pLogger_->Initialize();

    /// ウィンドウの初期化
    pWinSystem_->Initialize();
    pWinSystem_->ShowWnd();

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

    /// パーティクル基盤の初期化
    pParticleSystem_->Initialize();

    /// ライン基盤の初期化
    pLineSystem_->Initialize();

    /// テキスト基盤の初期化
    pTextSystem_->Initialize();

    /// オーディオの初期化
    pAudioManager_->Initialize();

    /// 入力の初期化
    pInput_->Initialize(GetModuleHandleA(nullptr), pWinSystem_->GetHwnd());

    /// 乱数生成器の初期化
    pRandomGenerator_->Initialize();

    /// SRVが初期化された後に呼ぶ
    pDirectX_->CreateGameScreenResource();

    /// ビューポートの初期化
    pViewport_ = std::make_unique<Viewport>();
    pViewport_->Initialize();

    /// シーンマネージャの初期化
    pSceneManager_->Initialize();
}

void NimaFramework::Finalize()
{
    pAudioManager_->Finalize();
    pImGuiManager_->Finalize();
    pWinSystem_->Finalize();
    pLogger_->Save();

    pSceneManager_->Finalize();
}

void NimaFramework::Update()
{
    UINT msg = pWinSystem_->GetMsg();
    if (msg == WM_QUIT)
    {
        isExitProgram_ = true;
        return;
    }

    if(pWinSystem_->IsResized())
    {
        pImGuiManager_->Resize();
    }

    /// マネージャ更新
    pInput_->Update();
    pModelManager_->Update();
    pAudioManager_->Update();
    pImGuiManager_->BeginFrame();
    pDebugManager_->Update();
    pDebugManager_->DrawUI();
    pViewport_->DrawWindow();
    pLogger_->DrawUI();

    /// シーン更新
    pSceneManager_->Update();

    /// ImGui更新
    pImGuiManager_->Render();

    /// パーティクル更新
    pParticleManager_->Update();
}

void NimaFramework::Draw()
{
    /// 背景スプライトの描画
    pSpriteSystem_->PresentDraw();
    pSceneManager_->SceneDraw2dBackGround();

    /// 3D描画
    pObject3dSystem_->DepthDrawSetting();
    pSceneManager_->SceneDraw3d();
    pObject3dSystem_->MainDrawSetting();
    pSceneManager_->SceneDraw3d();

    /// 中景スプライトの描画
    pSpriteSystem_->PresentDraw();
    pSceneManager_->SceneDraw2dMidground();

    /// 中景3dオブジェクトの描画
    pObject3dSystem_->MainDrawSetting();
    pSceneManager_->SceneDraw3dMidground();

    /// ライン描画
    pLineSystem_->PresentDraw();
    pSceneManager_->SceneDrawLine();

    /// パーティクル描画
    pParticleSystem_->PresentDraw();
    pParticleManager_->Draw();

    /// 前景スプライトの描画
    pSpriteSystem_->PresentDraw();
    pSceneManager_->SceneDraw2dForeground();

    /// レンダーターゲットからビューポート用リソースにコピー
    pDirectX_->CopyFromRTV();
    /// コンピュートシェーダーの実行
    pViewport_->Compute();

    /// ImGuiの描画
    pImGuiManager_->EndFrame();

    /// コマンドの実行
    pDirectX_->CommandExecute();

    /// テキストの描画
    pTextSystem_->PresentDraw();
    pSceneManager_->SceneDrawText();
    pTextSystem_->PostDraw();
}

void NimaFramework::PreProcess()
{
    pDirectX_->NewFrame();
    pSRVManager_->SetDescriptorHeaps();
}

void NimaFramework::PostProcess()
{
    pDirectX_->DisplayFrame();
}
