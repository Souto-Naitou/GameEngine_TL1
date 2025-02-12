#include "SampleProgram.h"

#include <Common/define.h>
#include <Windows.h>
#include <Features/SceneManager/SceneManager.h>
#include <Scene/Factory/SceneFactory.h>

void SampleProgram::Initialize()
{
    /// 基底クラスの初期化処理
    NimaFramework::Initialize();

    /// シーンファクトリの設定
    pSceneFactory_ = std::make_unique<SceneFactory>();
    pSceneManager_->SetSceneFactory(pSceneFactory_.get());

    /// 自動ロードパスの追加
    pModelManager_->AddAutoLoadPath("resources/temp");
    pTextureManager_->AddSearchPath("resources");

    /// モデルを全てロード
    pModelManager_->LoadAllModel();

    /// シーンの生成
    pSceneManager_->Initialize();
    pSceneManager_->ReserveScene("PG3PT1");
}

void SampleProgram::Finalize()
{
    /// 基底クラスの終了処理
    NimaFramework::Finalize();

    pSceneManager_->Finalize();
}

void SampleProgram::Update()
{
    /// 基底クラスの更新処理
    NimaFramework::Update();

    /// シーン更新
    pSceneManager_->Update();

    /// パーティクル更新
    pParticleManager_->Update();
}

void SampleProgram::Draw()
{
    /// 描画処理
    pDirectX_->PresentDraw();
    pSRVManager_->PresentDraw();

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

    //pDirectX_->CopyFromRTV();
    //pViewport_->Compute();

    pImGuiManager_->EndFrame();

    pDirectX_->CommandExecute();

    /// テキストの描画
    pTextSystem_->PresentDraw();
    pSceneManager_->SceneDrawText();
    pTextSystem_->PostDraw();

    pDirectX_->PostDraw();
}
