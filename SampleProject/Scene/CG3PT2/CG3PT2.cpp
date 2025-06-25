#include "CG3PT2.h"
#include <Features/Object3d/Object3dSystem.h>
#include <imgui.h>
#include <Features/Audio/AudioManager.h>
#include <Features/Input/Input.h>

#ifdef _DEBUG
#include <DebugTools/DebugManager/DebugManager.h>
#endif // _DEBUG

void CG3PT2::Initialize()
{
    /// カメラの初期化
    pGameEye_ = std::make_unique<GameEye>();
    pGameEye_->SetName("MainCamera@CG3PT2");
    pGameEye_->SetRotate({ 0.18f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 2.f, -7.f });

    /// オブジェクト共有にカメラをセット
    Object3dSystem::GetInstance()->SetGlobalEye(pGameEye_.get());

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
    pGrid_->SetEnableLighting(true);

    /// 平行光源
    directionalLight_.direction = { 0.0f, -1.0f, 0.0f };
    directionalLight_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    directionalLight_.intensity = 0.1f;

    pMonsterBall_->SetDirectionalLight(&directionalLight_);
    pGrid_->SetDirectionalLight(&directionalLight_);

    /// 点光源
    pointLight_.Initialize();
    pointLight_.enablePointLight = 1;
    pointLight_.position = { -1.5f, 0.25f, 2.0f };
    pointLight_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    pointLight_.intensity = 0.08f;

    pMonsterBall_->SetPointLight(&pointLight_);
    pGrid_->SetPointLight(&pointLight_);

    /// テキスト
    pText_ = std::make_unique<Text>();
    pText_->Initialize();
    pText_->SetName("Text@CG3PT2");
    pText_->SetText("CG3PT2");
    pText_->SetPosition({ 10.0f, 10.0f });
    pText_->SetMaxSize({ 100.0f, 100.0f });
    pText_->SetFontFamily("Meiryo");
    pText_->SetFontSize(15.0f);
    pText_->SetColorName("Black");

    pAudio_ = AudioManager::GetInstance()->GetNewAudio("piano.wav");
}

void CG3PT2::Finalize()
{
    pMonsterBall_->Finalize();
    pGrid_->Finalize();
    pointLight_.Finalize();
    pText_->Finalize();
}

void CG3PT2::Update()
{
    pointLight_.Update();
    pGameEye_->Update();
    pMonsterBall_->Update();
    pGrid_->Update();

    pText_->Update();

    if (Input::GetInstance()->TriggerKey(DIK_SPACE))
    {
        pAudio_->Play();
    }
}

void CG3PT2::Draw2dBackGround()
{
}

void CG3PT2::Draw3d()
{
    pointLight_.Draw();
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

void CG3PT2::DrawTexts()
{
    pText_->Draw();
}