#include "SampleProgram.h"

#include <Common/define.h>
#include <Windows.h>

void SampleProgram::Initialize()
{
    /// 基底クラスの初期化処理
    NimaFramework::Initialize();

    /// 自動ロードパスの追加
    pModelManager_->AddAutoLoadPath("resources/models");

    /// ゲーム内オブジェクトの宣言
    pObject3d_ = new Object3d();
    pGameEye_ = new GameEye();
    pSpriteMB_ = new Sprite();
    pSpriteUVC_ = new Sprite();
    pParticle_ = new Particle();
    pGameEye_->SetRotate({ 0.0f, -0.4f, 0.0f });
    pGameEye_->SetTranslate({ 5.0f, 0.0f, -10.0f });
    pGameEye_->SetName("MainCamera");

    /// 3Dオブジェクト基盤の初期化
    pObject3dSystem_->SetDefaultGameEye(pGameEye_);
    pParticleSystem_->Initialize();
    pParticleSystem_->SetDefaultGameEye(pGameEye_);

    /// ゲーム内オブジェクトの初期化
    pModelManager_->LoadAllModel();
    pObject3d_->Initialize(pObject3dSystem_, "suzanne.obj");
    pObject3d_->SetScale({ -1.0f, 1.0f, 1.0f });
    pObject3d_->SetName("Suzanne");

    pSpriteMB_->Initialize(pSpriteSystem_, "MonsterBall.png");
    pSpriteMB_->SetName("MonsterBall");
    pSpriteMB_->SetSize({ 120,60 });
    pSpriteMB_->SetPosition({ 40,60 });
    pSpriteUVC_->Initialize(pSpriteSystem_, "uvChecker.png");
    pSpriteUVC_->SetName("uvChecker");
    pSpriteUVC_->SetSize({ 120,120 });
    pSpriteUVC_->SetPosition({ 180, 60 });

    pParticle_->Initialize(pParticleSystem_, "plane.obj");
    pParticle_->SetName("Test");
    pParticle_->reserve(32);
    pParticle_->resize(15);
    for (uint32_t i = 0; i < pParticle_->size(); ++i)
    {
        (*pParticle_)[i].scale = { 1.0f, 1.0f, 1.0f };
        (*pParticle_)[i].rotate = { 0.0f, 0.0f, 0.0f };
        (*pParticle_)[i].translate = { 0.0f, 0.0f, static_cast<float>(i) };
    }

}

void SampleProgram::Finalize()
{
    /// 終了処理
    pSpriteUVC_->Finalize();
    pSpriteMB_->Finalize();
    pObject3d_->Finalize();
    pImGuiManager_->Finalize();
    pWin32App_->Finalize();

    /// ゲーム内オブジェクトの解放
    delete pParticle_;
    delete pSpriteUVC_;
    delete pSpriteMB_;
    delete pGameEye_;
    delete pObject3d_;

    /// 基底クラスの終了処理
    NimaFramework::Finalize();
}

void SampleProgram::Update()
{
    NimaFramework::Update();

    /// 更新処理
    pInput_->Update();
    pGameEye_->Update();
    pObject3d_->Update();
    pSpriteMB_->Update();
    pSpriteUVC_->Update();
    pParticle_->Update();
}

void SampleProgram::Draw()
{
    /// 描画処理
    pDirectX_->PresentDraw();
    pSRVManager_->PresentDraw();
    pSpriteSystem_->PresentDraw();
    pSpriteMB_->Draw();
    pSpriteUVC_->Draw();

    pObject3dSystem_->PresentDraw();
    pObject3d_->Draw();

    pParticleSystem_->PresentDraw();
    pParticle_->Draw();


    pImGuiManager_->EndFrame();
    pDirectX_->PostDraw();
}
