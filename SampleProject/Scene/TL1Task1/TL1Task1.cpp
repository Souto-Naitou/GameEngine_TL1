#include "TL1Task1.h"

#include <Features/LevelLoader/LevelHelper.h>

void TL1Task1::Initialize()
{
    pModelManager_ = std::any_cast<ModelManager*>(pArgs_->Get("ModelManager"));
    sceneObjects_ = Helper::Level::LoadScene("Resources/Json/Levels/test.json", pModelManager_);
}

void TL1Task1::Finalize()
{
    sceneObjects_.Finalize();
    pModelManager_ = nullptr;
}

void TL1Task1::Update()
{
    sceneObjects_.Update();
}

void TL1Task1::Draw()
{
    sceneObjects_.Draw();
}

void TL1Task1::DrawTexts()
{
}
