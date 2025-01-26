#include "PointLight.h"
#include <Features/Object3d/Object3d.h>

void PointLight::Initialize()
{
    pIcon_ = std::make_unique<Object3d>();
    pIcon_->Initialize("EngineResources/Models/PointLight/PointLight.obj");
    pIcon_->SetScale({ 0.05f, 0.05f, 0.05f });
    pIcon_->SetEnableLighting(true);
    pIcon_->SetPointLight(this);
}

void PointLight::Update()
{
    pIcon_->SetEnableLighting(false);
    pIcon_->SetTranslate(position);
    pIcon_->Update();
}

void PointLight::Draw()
{
    pIcon_->Draw();
}

void PointLight::Finalize()
{
    pIcon_->Finalize();
}
