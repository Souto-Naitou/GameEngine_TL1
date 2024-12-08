#include "RequiredScene.h"

#include <Features/Particle/ParticleManager.h>
#include <Features/SceneManager/SceneManager.h>

void RequiredScene::Initialize()
{
    pInput_ = Input::GetInstance();
    pGameEye_ = new GameEye();
    pGameEye_->SetName("RequiredScene_Camera");
    pGameEye_->SetTranslate(Vector3(0.0f, 0.0f, -10.0f));

    particle_ = &ParticleManager::GetInstance()->CreateParticle();
    particle_->Initialize("plane.obj");
    particle_->SetGameEye(pGameEye_);
    particle_->reserve(10);
    InitializeParticle();

}

void RequiredScene::Finalize()
{
    delete pGameEye_;
    particle_->Finalize();
    ParticleManager::GetInstance()->ReleaseParticle(particle_);
    particle_ = nullptr;
}

void RequiredScene::Update()
{
    pGameEye_->Update();

    if (pInput_->TriggerKey(DIK_0))
    {
        SceneManager::GetInstance()->ReserveScene("GameScene");
    }

    particle_->Update();
}

void RequiredScene::Draw2dBackGround()
{

}

void RequiredScene::Draw3d()
{

}

void RequiredScene::Draw2dForeground()
{

}

void RequiredScene::InitializeParticle()
{
    for (int i = 9; i >= 0; --i)
    {
        ParticleData data;
        data.transform_.translate = Vector3(i * 0.1f, i * 0.1f, i * 0.1f);
        data.transform_.scale = Vector3(1.0f, 1.0f, 1.0f);
        data.transform_.rotate = Vector3(0.0f, 0.0f, 0.0f);
        data.color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        data.velocity_ = Vector3(0.0f, 0.0f, 0.0f);
        data.acceleration_ = Vector3(0.0f, 0.0f, 0.0f);
        data.alphaDeltaValue_ = 0.0f;
        data.lifeTime_ = 0.0f;
        data.deleteCondition_ = ParticleDeleteCondition::ZeroAlpha;

        particle_->emplace_back(data);
    }
}
