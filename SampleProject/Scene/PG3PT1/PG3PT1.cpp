#include "PG3PT1.h"

#include <Features/GameEye/FreeLook/FreeLookEye.h>

void PG3PT1::Initialize()
{
    pInput_ = Input::GetInstance();

    pGameEye_ = std::make_unique<FreeLookEye>();
    pBunny_ = std::make_unique<Object3d>();

    directionalLight_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    directionalLight_.direction = { 0.0f, -1.0f, 0.0f };
    directionalLight_.intensity = 0.1f;

    pGameEye_->SetTranslate({ 0.0f, 0.0f, -5.0f });
    pGameEye_->SetRotate({ 0.0f, 0.0f, 0.0f });

    pBunny_->Initialize("Bunny.obj");
    pBunny_->SetTranslate({ 0.0f, -0.7f, 0.0f });
    pBunny_->SetRotate({ 0.0f, 0.0f, 0.0f });
    pBunny_->SetScale({ 1.0f, 1.0f, 1.0f });
    pBunny_->SetEnableLighting(true);
    pBunny_->SetGameEye(pGameEye_.get());
    pBunny_->SetDirectionalLight(&directionalLight_);
}

void PG3PT1::Finalize()
{
    pBunny_->Finalize();
}

void PG3PT1::Update()
{
    if ( pInput_->PushKey(DIK_SPACE) )
    {
        pGameEye_->Shake(0.01f);
    }

    pGameEye_->Update();
    pBunny_->Update();
}

void PG3PT1::Draw2dBackGround()
{
}

void PG3PT1::Draw3d()
{
    pBunny_->Draw();
}

void PG3PT1::Draw2dMidground()
{
}

void PG3PT1::Draw3dMidground()
{
}

void PG3PT1::DrawLine()
{
}

void PG3PT1::Draw2dForeground()
{
}

void PG3PT1::DrawTexts()
{
}