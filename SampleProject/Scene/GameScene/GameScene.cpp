#include "GameScene.h"

#include <Features/Model/ModelManager.h>
#include <Features/Sprite/SpriteSystem.h>
#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/SceneManager/SceneManager.h>
#include <Features/Line/LineSystem.h>
#include <Core/Win32/WinSystem.h>


void GameScene::Initialize()
{
    pInput_ = Input::GetInstance();

    pFirework_ = new ParticleEmitter();
    pSmoke_ = new ParticleEmitter();
    pSpark_ = new ParticleEmitter();


    pGameEye_ = new GameEye();
    pSkydome_ = new Object3d();
    pGrid_ = new Object3d();

    pGuideSprite_ = new Sprite();
    pGuideSprite_->Initialize("Text/SceneChangeGuide.png");
    pGuideSprite_->SetName("GuideText");
    pGuideSprite_->SetPosition(Vector2(WinSystem::kClientWidth - 40.0f, WinSystem::kClientHeight - 40.0f));
    pGuideSprite_->SetAnchorPoint({ 1,1 });

    pGameEye_->SetRotate({ 0.1f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 0.2f, -5.0f });
    pGameEye_->SetName("MainCamera");

    /// システムにデフォルトのゲームカメラを設定
    Object3dSystem::GetInstance()->SetSharedGameEye(pGameEye_);
    ParticleSystem::GetInstance()->SetDefaultGameEye(pGameEye_);
    LineSystem::GetInstance()->SetDefaultGameEye(pGameEye_);


    pSkydome_->Initialize("Skydome.obj");
    pSkydome_->SetScale({ 1.0f, 1.0f, 1.0f });
    pSkydome_->SetName("Skydome");
    pSkydome_->SetEnableLighting(false);

    pGrid_->Initialize("Grid_v3.obj");
    pGrid_->SetScale({ 1.0f, 1.0f, 1.0f });
    pGrid_->SetName("Grid");
    pGrid_->SetTilingMultiply({ 100.0f, 100.0f });
    pGrid_->SetEnableLighting(false);

    /// エミッタの初期化
    pFirework_->Initialize("Particle/ParticleSpark.obj", "Resources/Json/Firework.json");
    pSmoke_->Initialize("Particle/ParticleSpark.obj", "Resources/Json/Smoke.json");
    pSpark_->Initialize("Particle/ParticleSpark.obj", "Resources/Json/Spark.json");
}

void GameScene::Finalize()
{
    pSkydome_->Finalize();
    pGrid_->Finalize();
    pGuideSprite_->Finalize();
    pFirework_->Finalize();
    pSmoke_->Finalize();
    pSpark_->Finalize();

    delete pFirework_;
    delete pSmoke_;
    delete pSpark_;
    delete pGuideSprite_;
    delete pGrid_;
    delete pSkydome_;
    delete pGameEye_;
}

void GameScene::Update()
{
    if (pInput_->PushKey(DIK_LCONTROL) && pInput_->PushKey(DIK_1))
    {
        SceneManager::GetInstance()->ReserveScene("RequiredScene");
    }

    /// 更新処理
    pGameEye_->Update();
    pGuideSprite_->Update();
    pSkydome_->Update();
    pGrid_->Update();
    pFirework_->Update();
    pSmoke_->Update();
    pSpark_->Update();
}

void GameScene::Draw2dBackGround()
{
}

void GameScene::Draw3dMidground()
{
}

void GameScene::Draw2dMidground()
{
}

void GameScene::DrawLine()
{
    pFirework_->Draw();
    pSmoke_->Draw();
    pSpark_->Draw();
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