#include "GameScene.h"

void GameScene::Initialize()
{
    pParticleSystem_ = ParticleSystem::GetInstance();

    pObject3d_ = new Object3d();
    pGameEye_ = new GameEye();
    pSpriteMB_ = new Sprite();
    pSpriteUVC_ = new Sprite();
    pParticle_ = new Particle();

    pGameEye_->SetRotate({ 0.0f, -0.4f, 0.0f });
    pGameEye_->SetTranslate({ 5.0f, 0.0f, -10.0f });
    pGameEye_->SetName("MainCamera");

    Object3dSystem::GetInstance()->SetDefaultGameEye(pGameEye_);
    pParticleSystem_->SetDefaultGameEye(pGameEye_);

    pObject3d_->Initialize("suzanne.obj");
    pObject3d_->SetScale({ -1.0f, 1.0f, 1.0f });
    pObject3d_->SetName("Suzanne");

    pSpriteMB_->Initialize("MonsterBall.png");
    pSpriteMB_->SetName("MonsterBall");
    pSpriteMB_->SetSize({ 120,60 });
    pSpriteMB_->SetPosition({ 40,60 });
    pSpriteUVC_->Initialize("uvChecker.png");
    pSpriteUVC_->SetName("uvChecker");
    pSpriteUVC_->SetSize({ 120,120 });
    pSpriteUVC_->SetPosition({ 180, 60 });

    pParticle_->Initialize("plane.obj");
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

void GameScene::Finalize()
{
    pSpriteUVC_->Finalize();
    pSpriteMB_->Finalize();
    pObject3d_->Finalize();

    delete pParticle_;
    delete pSpriteUVC_;
    delete pSpriteMB_;
    delete pGameEye_;
    delete pObject3d_;
}

void GameScene::Update()
{
    /// 更新処理
    pGameEye_->Update();
    pObject3d_->Update();
    pSpriteMB_->Update();
    pSpriteUVC_->Update();
    pParticle_->Update();
}

void GameScene::Draw2dBackGround()
{
}

void GameScene::Draw3d()
{
    pObject3d_->Draw();

    pParticleSystem_->PresentDraw();
    pParticle_->Draw();

}

void GameScene::Draw2dForeground()
{
    pSpriteMB_->Draw();
    pSpriteUVC_->Draw();
}
