#include "DepthScene.h"

#include <DebugTools/DebugManager/DebugManager.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include <Features/Model/ModelManager.h>

void DepthScene::Initialize()
{
    pInput_ = Input::GetInstance();

    // モデル管理クラスの取得
    pModelManager_ = std::any_cast<ModelManager*>(pArgs_->Get("ModelManager"));

    pModelPlane_ = pModelManager_->Load("plane.obj");

#ifdef _DEBUG
    DebugManager::GetInstance()->SetComponent("Scene", windowName_, std::bind(&DepthScene::DebugWindow, this));
#endif // _DEBUG

    pGameEye_ = std::make_unique<GameEye>();
    pGameEye_->SetName("DepthScene_Camera");
    pGameEye_->SetTranslate(Vector3(0.0f, 0.0f, -10.0f));

    CreateObject();
}

void DepthScene::Finalize()
{
    objectList_.clear();

#ifdef _DEBUG
    DebugManager::GetInstance()->DeleteComponent("Scene", windowName_);
#endif // _DEBUG
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

void DepthScene::Draw()
{
    // =============================================
    // [Object3d Begin]
    for ( auto& obj : objectList_ )
    {
        obj->Draw();
    }
    // [Object3d End]
    // =============================================
}

void DepthScene::DrawTexts()
{
}

void DepthScene::CreateObject()
{
    objectList_.push_back(std::make_unique<Object3d>());
    Object3d* obj = objectList_.back().get();
    obj->Initialize();
    obj->SetTranslate(Vector3(0.0f, 0.0f, nextObjZ_));
    obj->SetGameEye(pGameEye_.get());
    obj->SetModel(pModelPlane_);
    nextObjZ_ += objZInterval_;
}

void DepthScene::DebugWindow()
{
#ifdef _DEBUG

    ImGui::Text("Object count : %d", objectList_.size());

    /// 一括作成の個数の入力
    ImGui::InputInt("Create Count", &nextCreateCountTemp_);

    /// 一括作成の実行
    if ( ImGui::Button("Create") )
    {
        nextCreateCount_ = nextCreateCountTemp_;
    }

#endif // _DEBUG
}
