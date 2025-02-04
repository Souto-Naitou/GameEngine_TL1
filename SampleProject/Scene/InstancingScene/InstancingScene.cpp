#include "InstancingScene.h"

#include <Features/Particle/ParticleManager.h>
#include <Features/SceneManager/SceneManager.h>

void InstancingScene::Initialize()
{
    pInput_ = Input::GetInstance();
    pGameEye_ = std::make_unique<GameEye>();
    pGameEye_->SetName("InstancingScene_Camera");
    pGameEye_->SetTranslate(Vector3(0.0f, 0.0f, -10.0f));

    pGuideSprite_ = std::make_unique<Sprite>();
    pGuideSprite_->Initialize("Text/SceneChangeGuide.png");
    pGuideSprite_->SetName("GuideText");
    pGuideSprite_->SetPosition(Vector2(1280.0f - 40.0f, 720.0f - 40.0f));
    pGuideSprite_->SetAnchorPoint({ 1,1 });

    particle_ = &ParticleManager::GetInstance()->CreateParticle();
    particle_->Initialize("plane.obj");
    particle_->SetGameEye(pGameEye_.get());
    particle_->reserve(10);
    InitializeParticle();
}

void InstancingScene::Finalize()
{
    pGuideSprite_->Finalize();


    ParticleManager::GetInstance()->ReleaseParticle(particle_);
    particle_ = nullptr;
}

void InstancingScene::Update()
{
    pGameEye_->Update();
    pGuideSprite_->Update();

    if (pInput_->PushKey(DIK_LCONTROL) && pInput_->TriggerKey(DIK_2))
    {
        SceneManager::GetInstance()->ReserveScene("GameScene");
    }

    particle_->Update();
}

void InstancingScene::Draw2dBackGround()
{

}

void InstancingScene::Draw3d()
{

}

void InstancingScene::Draw2dMidground()
{
}

void InstancingScene::Draw3dMidground()
{
}

void InstancingScene::DrawLine()
{
}

void InstancingScene::Draw2dForeground()
{
    pGuideSprite_->Draw();
}

void InstancingScene::DrawTexts()
{
}

void InstancingScene::InitializeParticle()
{
    for (int i = 9; i >= 0; --i)
    {
        ParticleData data;
        data.transform_.translate = Vector3(i * 0.1f, i * 0.1f, i * 0.1f);
        data.transform_.scale = Vector3(1.0f, 1.0f, 1.0f);
        data.startScale_ = Vector3(1.0f, 1.0f, 1.0f);
        data.transform_.rotate = Vector3(0.0f, 0.0f, 0.0f);
        data.beginColor_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        data.endColor_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        data.velocity_ = Vector3(0.0f, 0.0f, 0.0f);
        data.acceleration_ = Vector3(0.0f, 0.0f, 0.0f);
        data.alphaDeltaValue_ = 0.0f;
        data.lifeTime_ = 0.0f;
        data.deleteCondition_ = ParticleDeleteCondition::ZeroAlpha;

        particle_->emplace_back(data);
    }
}
