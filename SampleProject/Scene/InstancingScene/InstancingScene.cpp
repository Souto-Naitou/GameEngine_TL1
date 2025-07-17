#include "InstancingScene.h"

#include <Features/Particle/ParticleManager.h>
#include <Features/SceneManager/SceneManager.h>
#include <Range.h>

void InstancingScene::Initialize()
{
    pInput_ = Input::GetInstance();
    pGameEye_ = std::make_unique<GameEye>();
    pGameEye_->SetName("InstancingScene_Camera");
    pGameEye_->SetTranslate(Vector3(0.0f, 0.0f, -10.0f));
    ParticleSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    
    // モデルマネージャーの取得
    pModelManager_ = std::any_cast<ModelManager*>(pArgs_->Get("ModelManager"));

    particle_ = ParticleManager::GetInstance()->CreateParticle();
    particle_->Initialize(pModelManager_->Load("plane.obj"));
    particle_->reserve(10);
    InitializeParticle();
}

void InstancingScene::Finalize()
{
    ParticleManager::GetInstance()->ReleaseParticle(particle_);
    particle_ = nullptr;
}

void InstancingScene::Update()
{
    pGameEye_->Update();

    if (pInput_->PushKey(DIK_LCONTROL) && pInput_->TriggerKey(DIK_2))
    {
        SceneManager::GetInstance()->ReserveScene("GameScene");
    }

    particle_->Update();
}

void InstancingScene::Draw()
{
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
        data.scaleRange_.start() = Vector3(1.0f, 1.0f, 1.0f);
        data.transform_.rotate = Vector3(0.0f, 0.0f, 0.0f);
        data.colorRange_ = Range(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        data.velocity_ = Vector3(0.0f, 0.0f, 0.0f);
        data.acceleration_ = Vector3(0.0f, 0.0f, 0.0f);
        data.alphaDeltaValue_ = 0.0f;
        data.lifeTime_ = 0.0f;
        data.deleteCondition_ = ParticleDeleteCondition::ZeroAlpha;

        particle_->emplace_back(data);
    }
}
