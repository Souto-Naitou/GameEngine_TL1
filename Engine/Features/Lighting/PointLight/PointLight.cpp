#include "PointLight.h"
#include <Features/Object3d/Object3d.h>

#include <sstream>
#include <cstdarg>

void PointLight::Initialize()
{
    pIcon_ = std::make_unique<Object3d>();
    pIcon_->Initialize();
    pIcon_->SetScale({ 0.05f, 0.05f, 0.05f });
    pIcon_->SetEnableLighting(true);
    pIcon_->SetPointLight(this);

    std::stringstream ss;
    ss << "PointLight##0x" << std::hex << this;
    pIcon_->SetName(ss.str());
}

void PointLight::Update()
{
    pIcon_->SetEnableLighting(false);
    pIcon_->SetTranslate(position);
    pIcon_->Update();
}

void PointLight::Draw()
{
    pIcon_->Draw(pIconModel_);
}

void PointLight::Finalize()
{
    pIcon_->Finalize();
}

void PointLight::SetIconModel(IModel* pModel)
{
    pIconModel_ = pModel;
}
