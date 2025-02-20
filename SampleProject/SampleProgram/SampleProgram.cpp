#include "SampleProgram.h"

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
    pModelManager_->AddAutoLoadPath("resources/models");
    pModelManager_->AddAutoLoadPath("resources/temp");
    pTextureManager_->AddSearchPath("resources");


    /// モデルを全てロード
    pModelManager_->LoadAllModel();


    /// シーンの生成
    pSceneManager_->ReserveScene("PG3PT1");
}

void SampleProgram::Finalize()
{
    /// 基底クラスの終了処理
    NimaFramework::Finalize();
}

void SampleProgram::Update()
{
    /// 基底クラスの更新処理
    NimaFramework::Update();
}

void SampleProgram::Draw()
{
    /// 描画前処理
    NimaFramework::PreProcess();


    /// バックバッファ書き込み
    NimaFramework::Draw();


    /// 描画後処理
    NimaFramework::PostProcess();
}
