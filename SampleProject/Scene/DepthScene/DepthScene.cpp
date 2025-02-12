#include "DepthScene.h"

#include <DebugTools/DebugManager/DebugManager.h>
#include <imgui.h>

void DepthScene::Initialize()
{
    pInput_ = Input::GetInstance();
    DebugManager::GetInstance()->SetComponent("Scene", windowName_, std::bind(&DepthScene::DebugWindow, this));

    pGameEye_ = std::make_unique<GameEye>();
    pGameEye_->SetName("DepthScene_Camera");
    pGameEye_->SetTranslate(Vector3(0.0f, 0.0f, -10.0f));

    CreateObject();
}

void DepthScene::Finalize()
{
    objectList_.clear();

    DebugManager::GetInstance()->DeleteComponent("Scene", windowName_.c_str());
}

void DepthScene::Update()
{
    if ( pInput_->TriggerKey(DIK_RETURN) )
    {
        CreateObject();
    }

    if ( nextCreateCount_ > 0 )
    {
        CreateObject();
        nextCreateCount_--;
    }

    for ( auto& obj : objectList_ )
    {
        obj->Update();
    }

    pGameEye_->Update();
}

void DepthScene::Draw2dBackGround()
{
}

void DepthScene::Draw3d()
{
    for ( auto& obj : objectList_ )
    {
        obj->Draw();
    }
}

void DepthScene::Draw2dMidground()
{
}

void DepthScene::Draw3dMidground()
{
}

void DepthScene::DrawLine()
{
}

void DepthScene::Draw2dForeground()
{
}

void DepthScene::DrawTexts()
{
}

void DepthScene::CreateObject()
{
    objectList_.push_back(std::make_unique<Object3d>());
    Object3d* obj = objectList_.back().get();
    obj->Initialize("plane.obj");
    obj->SetTranslate(Vector3(0.0f, 0.0f, nextObjZ_));
    obj->SetGameEye(pGameEye_.get());
    nextObjZ_ += objZInterval_;
}

void DepthScene::DebugWindow()
{
    ImGui::Text("Object count : %d", objectList_.size());

    /// 一括作成の個数の入力
    ImGui::InputInt("Create Count", &nextCreateCountTemp_);

    /// 一括作成の実行
    if ( ImGui::Button("Create") )
    {
        nextCreateCount_ = nextCreateCountTemp_;
    }
}
