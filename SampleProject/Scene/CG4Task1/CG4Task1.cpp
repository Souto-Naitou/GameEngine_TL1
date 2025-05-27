#include "CG4Task1.h"

#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/SceneManager/SceneManager.h>
#include <Features/Line/LineSystem.h>
#include <Features/GameEye/FreeLook/FreeLookEye.h>


void CG4Task1::Initialize()
{
    pInput_ = Input::GetInstance();

    pGameEye_ = std::make_unique<FreeLookEye>();
    pGameEye_->SetRotate({ 0.1f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 0.2f, -5.0f });
    pGameEye_->SetName("MainCamera");

    /// システムにデフォルトのゲームカメラを設定
    Object3dSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    ParticleSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    LineSystem::GetInstance()->SetGlobalEye(pGameEye_.get());

    pGrid_ = std::make_unique<Object3d>();
    pGrid_->Initialize("Grid_v4.obj");
    pGrid_->SetScale({ 1.0f, 1.0f, 1.0f });
    pGrid_->SetName("Grid");
    pGrid_->SetTilingMultiply({ 100.0f, 100.0f });
    pGrid_->SetEnableLighting(false);

    pParticleEmitter_ = std::make_unique<ParticleEmitter>();
    pParticleEmitter_->Initialize("Particle/ParticleSpark.obj", "circle2.png");
}

void CG4Task1::Finalize()
{
    pParticleEmitter_->Finalize();
    pGrid_->Finalize();
}

void CG4Task1::Update()
{
    /// 更新処理
    pGameEye_->Update();
    pGrid_->Update();
    pParticleEmitter_->Update();
}

void CG4Task1::Draw2dBackGround()
{
}

void CG4Task1::Draw3dMidground()
{
}

void CG4Task1::Draw2dMidground()
{
}

void CG4Task1::DrawLine()
{
    pParticleEmitter_->Draw();
}

void CG4Task1::Draw3d()
{    
    pGrid_->Draw();
}

void CG4Task1::Draw2dForeground()
{
}

void CG4Task1::DrawTexts()
{
}
