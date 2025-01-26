#include "CG3PT2.h"
#include <Features/Object3d/Object3dSystem.h>

void CG3PT2::Initialize()
{
    /// カメラの初期化
    pGameEye_ = std::make_unique<GameEye>();
    pGameEye_->SetName("MainCamera@CG3PT2");
    pGameEye_->SetRotate({ 0.18f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 2.f, -7.f });

    /// オブジェクト共有にカメラをセット
    Object3dSystem::GetInstance()->SetSharedGameEye(pGameEye_.get());

    pMonsterBall_ = std::make_unique<Object3d>();
    pMonsterBall_->Initialize("Sphere1m/Sphere1m.obj");
    pMonsterBall_->SetScale({ 1.0f, 1.0f, 1.0f });
    pMonsterBall_->SetRotate({ 0.0f, -3.14f / 2.0f, 0.0f });
    pMonsterBall_->SetTranslate({ 0.0f, 1.f, 0.0f });
    pMonsterBall_->SetName("MonsterBall");
    pMonsterBall_->SetEnableLighting(true);
    pMonsterBall_->SetShininess(12.f);

    pGrid_ = std::make_unique<Object3d>();
    pGrid_->Initialize("Grid_v3.obj");
    pGrid_->SetScale({ 1.0f, 1.0f, 1.0f });
    pGrid_->SetName("Grid");
    pGrid_->SetTilingMultiply({ 100.0f, 100.0f });
    pGrid_->SetEnableLighting(false);

    /// ライト
    directionalLight_.direction = { 0.0f, -1.0f, 0.0f };
    directionalLight_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    directionalLight_.intensity = 1.0f;

    pMonsterBall_->SetDirectionalLight(&directionalLight_);
    pGrid_->SetDirectionalLight(&directionalLight_);

}

void CG3PT2::Finalize()
{
    pMonsterBall_->Finalize();
}

void CG3PT2::Update()
{
    pGameEye_->Update();
    pMonsterBall_->Update();
    pGrid_->Update();
}

void CG3PT2::Draw2dBackGround()
{
}

void CG3PT2::Draw3d()
{
    pMonsterBall_->Draw();
    pGrid_->Draw();
}

void CG3PT2::Draw2dMidground()
{
}

void CG3PT2::Draw3dMidground()
{
}

void CG3PT2::DrawLine()
{
}

void CG3PT2::Draw2dForeground()
{
}
