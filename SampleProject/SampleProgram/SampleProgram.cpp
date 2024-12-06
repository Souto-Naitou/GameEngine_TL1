#include "SampleProgram.h"

#include <Common/define.h>
#include <Windows.h>
#include <Features/SceneManager/SceneManager.h>
#include <Scene/GameScene.h>

void SampleProgram::Initialize()
{
    /// 基底クラスの初期化処理
    NimaFramework::Initialize();

    /// 自動ロードパスの追加
    pModelManager_->AddAutoLoadPath("resources/models");

    /// モデルを全てロード
    pModelManager_->LoadAllModel();

    /// 初期シーンの設定
    pSceneManager_->ChangeScene(std::make_unique<GameScene>());
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
    pSceneManager_->SceneUpdate();
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
    pObject3dSystem_->PresentDraw();
    pSceneManager_->SceneDraw3d();


    /// 前景スプライトの描画
    // pSpriteSystem_->PresentDraw();

    //pParticleSystem_->PresentDraw();


    pImGuiManager_->EndFrame();
    pDirectX_->PostDraw();
}
