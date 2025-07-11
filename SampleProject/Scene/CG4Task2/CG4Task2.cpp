#include "./CG4Task2.h"

#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/Line/LineSystem.h>
#include <Features/GameEye/FreeLook/FreeLookEye.h>

#include <DebugTools/DebugManager/DebugManager.h>
#include <Features/Model/ObjModel.h>
#include <Features/Model/GltfModel.h>


void CG4Task2::Initialize()
{
    pInput_ = Input::GetInstance();
    pModelManager_ = std::any_cast<ModelManager*>(pArgs_->Get("ModelManager"));

    pGameEye_ = std::make_unique<FreeLookEye>();
    pGameEye_->SetRotate({ 0.1f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 0.2f, -5.0f });
    pGameEye_->SetName("MainCamera");

    /// システムにデフォルトのゲームカメラを設定
    Object3dSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    ParticleSystem::GetInstance()->SetGlobalEye(pGameEye_.get());
    LineSystem::GetInstance()->SetGlobalEye(pGameEye_.get());

    // モデルの初期化
    auto pSrc = pModelManager_->Load("Human/sneakWalk.gltf");
    pModelSimple_ = std::make_unique<GltfModel>();
    pModelSimple_->Initialize();
    pModelSimple_->Clone(pSrc);

    // グリッドの初期化
    pModelGrid_ = std::make_unique<ObjModel>();
    pModelGrid_->Clone(pModelManager_->Load("Grid_v4/Grid_v4.obj"));
    pGrid_ = std::make_unique<Object3d>();
    pGrid_->Initialize();
    pGrid_->SetScale({ 1.0f, 1.0f, 1.0f });
    pGrid_->SetName("Grid");
    pGrid_->SetTilingMultiply({ 100.0f, 100.0f });
    pGrid_->SetEnableLighting(false);
    pGrid_->SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
    pGrid_->SetModel(pModelGrid_.get());

    pSimple_ = std::make_unique<Object3d>();
    pSimple_->Initialize();
    pSimple_->SetScale({ 1.0f, 1.0f, 1.0f });
    pSimple_->SetName("sneakWalk");
    pSimple_->SetModel(pModelSimple_.get());

    pText_ = std::make_unique<Text>();
    pText_->Initialize();
    pText_->SetName("FPSTEXT");
    pText_->SetFontFamily("Bahnschrift");
    pText_->SetFontSize(12);
    pText_->SetColorName("White");
    pText_->SetPosition({ 10.0f, 10.0f });
    pText_->SetText("FPS: 0.0");
    pText_->SetMaxSize({ 200.0f, 50.0f });
}

void CG4Task2::Finalize()
{
    pText_->Finalize();
    pGrid_->Finalize();
    pSimple_->Finalize();
}

void CG4Task2::Update()
{
    /// 更新処理
    pGameEye_->Update();

    pModelGrid_->Update();
    pModelSimple_->Update();

    pGrid_->Update();
    pSimple_->Update();

    pText_->SetText("FPS: " + std::to_string(static_cast<int>(DebugManager::GetInstance()->GetFPS())));
    pText_->Update();
}

void CG4Task2::Draw2dBackGround()
{
}

void CG4Task2::DrawLine()
{
    auto ptr = dynamic_cast<GltfModel*>(pModelSimple_.get());
    ptr->GetSkeleton()->DrawLine();
}

void CG4Task2::Draw3d()
{    
    pGrid_->Draw();
    pSimple_->Draw();
}

void CG4Task2::Draw2dForeground()
{
}

void CG4Task2::DrawTexts()
{
    pText_->Draw();
}
