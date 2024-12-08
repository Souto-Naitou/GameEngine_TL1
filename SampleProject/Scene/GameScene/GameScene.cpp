#include "GameScene.h"

#include <Features/Model/ModelManager.h>
#include <Features/Sprite/SpriteSystem.h>
#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/SceneManager/SceneManager.h>


void GameScene::Initialize()
{
    pInput_ = Input::GetInstance();

    pGameEye_ = new GameEye();
    pParticleEmitter_ = new ParticleEmitter();
    pSkydome_ = new Object3d();
    pGrid_ = new Object3d();

    pGuideSprite_ = new Sprite();
    pGuideSprite_->Initialize("Text/Guide.png");
    pGuideSprite_->SetName("GuideText");
    pGuideSprite_->SetPosition(Vector2(10.0f, 10.0f));
    pGuideSprite_->SetAnchorPoint({ 0,0 });

    pGameEye_->SetRotate({ 0.1f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 0.2f, -20.0f });
    pGameEye_->SetName("MainCamera");

    /// システムにデフォルトのゲームカメラを設定
    Object3dSystem::GetInstance()->SetDefaultGameEye(pGameEye_);
    ParticleSystem::GetInstance()->SetDefaultGameEye(pGameEye_);

    ModelManager::GetInstance()->SetLightingFlag("Skydome.obj", false);

    pSkydome_->Initialize("Skydome.obj");
    pSkydome_->SetScale({ 1.0f, 1.0f, 1.0f });
    pSkydome_->SetName("Skydome");

    pGrid_->Initialize("Grid.obj");
    pGrid_->SetScale({ 1.0f, 1.0f, 1.0f });
    pGrid_->SetName("Grid");
    pGrid_->SetTilingMultiply({ 90.0f, 90.0f });

    /// エミッタの初期化
    pParticleEmitter_->Initialize("Particle/ParticleSpark.obj");

    EmitterSetting();
}

void GameScene::Finalize()
{
    pSkydome_->Finalize();
    pGrid_->Finalize();
    pParticleEmitter_->Finalize();
    pGuideSprite_->Finalize();

    delete pGuideSprite_;
    delete pGrid_;
    delete pSkydome_;
    delete pParticleEmitter_;
    delete pGameEye_;
}

void GameScene::Update()
{
    if (pInput_->TriggerKey(DIK_1))
    {
        SceneManager::GetInstance()->ReserveScene("RequiredScene");
    }

    /// 更新処理
    pGameEye_->Update();
    pParticleEmitter_->Update();
    pGuideSprite_->Update();
    pSkydome_->Update();
    pGrid_->Update();
}

void GameScene::Draw2dBackGround()
{
}

void GameScene::Draw3d()
{
    pSkydome_->Draw();
    pGrid_->Draw();
}

void GameScene::Draw2dForeground()
{
    pGuideSprite_->Draw();
}

void GameScene::EmitterSetting()
{
    auto& emitterData = pParticleEmitter_->GetEmitterData();
    emitterData.scale_ = Vector3(0.1f, 0.1f, 0.1f);
    emitterData.emitInterval_ = 0.2f;
    emitterData.emitNum_ = 3;
    emitterData.emitterLifeTime_ = 0.0f;
    emitterData.enableRandomEmit_ = false;
    emitterData.emitPositionFixed_ = Vector3(0.0f, 0.0f, 0.0f);
    emitterData.color_ = Vector4(0.8f, 0.35f, 0.15f, 1.0f);
    emitterData.emitterLifeTime_ = 0.0f;
    emitterData.enableRandomVelocity_ = true;
    emitterData.velocityRandomRangeBegin_ = Vector3(-2.0f, -0.0f, -0.0f);
    emitterData.velocityRandomRangeEnd_ = Vector3(0.0f, 0.0f, 0.0f);
    emitterData.gravity_ = Vector3(0.0f, -0.9f, 0.0f);
    emitterData.resistance_ = Vector3(0.0f, 0.0f, 0.0f);
}
