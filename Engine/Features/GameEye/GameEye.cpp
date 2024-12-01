#include "GameEye.h"
#include <Core/Win32/Win32Application.h>
#include <DebugTools/DebugManager/DebugManager.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

GameEye::GameEye()
    : transform_({ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f) })
    , fovY_(0.45f)
    , aspectRatio_(static_cast<float>(Win32Application::kClientWidth) / static_cast<float>(Win32Application::kClientHeight))
    , nearClip_(0.1f)
    , farClip_(100.0f)
    , wMatrix_(Matrix4x4::AffineMatrix(transform_.scale, transform_.rotate, transform_.translate))
    , vMatrix_(wMatrix_.Inverse())
    , pMatrix_(Matrix4x4::PerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_))
    , vpMatrix_(vMatrix_* pMatrix_)
{
    DebugManager::GetInstance()->SetComponent("GameEye", name_, std::bind(&GameEye::DebugWindow, this));
}

GameEye::~GameEye()
{
    DebugManager::GetInstance()->DeleteComponent("GameEye", name_.c_str());
}

void GameEye::Update()
{
    wMatrix_ = Matrix4x4::AffineMatrix({ 1.0f, 1.0f, 1.0f }, transform_.rotate, transform_.translate);
    vMatrix_ = wMatrix_.Inverse();
    pMatrix_ = Matrix4x4::PerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
    vpMatrix_ = vMatrix_ * pMatrix_;
}

void GameEye::DebugWindow()
{
#ifdef _DEBUG

    ImGui::SeparatorText("Transform");
    ImGui::PushID("TRANSFORM");
    ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
    ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
    ImGui::DragFloat3("Translate", &transform_.translate.x, 0.01f);
    ImGui::PopID();

    ImGui::SeparatorText("Perspective options");
    ImGui::PushID("PERSPECTIVE");
    ImGui::DragFloat("FovY", &fovY_, 0.01f);
    ImGui::DragFloat("Aspect Ratio", &aspectRatio_, 0.01f);
    ImGui::DragFloat("Near Clip", &nearClip_, 0.01f);
    ImGui::DragFloat("Far Clip", &farClip_, 0.01f);
    ImGui::PopID();

#endif
}
