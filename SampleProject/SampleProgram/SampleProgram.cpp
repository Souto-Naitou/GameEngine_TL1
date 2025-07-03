#include "SampleProgram.h"

#include <Features/SceneManager/SceneManager.h>
#include <Scene/Factory/SceneFactory.h>


void SampleProgram::Initialize()
{
    /// 基底クラスの初期化処理
    NimaFramework::Initialize();

    #ifdef _DEBUG
    pImGuiManager_->EnableDocking();
    //pImGuiManager_->EnableMultiViewport();
    //pImGuiManager_->StyleMaterialFlat();
    #endif // _DEBUG
    
    /// シーンファクトリの設定
    pSceneFactory_ = std::make_unique<SceneFactory>();
    pSceneManager_->SetSceneFactory(pSceneFactory_.get());

    /// モデルを全てロード
    pModelManager_->LoadAllModel();

    pTextureManager_->LoadTexture("noise0.png");
    pPEDissolve_->SetTextureResource(pTextureManager_->GetTextureResource("noise0.png"));

    // 計測スタート
    globalTimer_.Start();
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
    pPERandomFilter_->SetSeed(globalTimer_.GetNow<float>() * 0.01f);
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
