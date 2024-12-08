#include "BaseParticleEmitter.h"

#ifdef _DEBUG
#include <DebugTools/DebugManager/DebugManager.h>
#include <DebugTools/ImGuiTemplates/ImGuiTemplates.h>
#include <sstream>
#endif // _DEBUG

void BaseParticleEmitter::Initialize([[maybe_unused]] const std::string& _filePath)
{
#ifdef _DEBUG
    std::stringstream ss;
    ss << "instance##0x" << std::hex << this;
    name_ = ss.str();
    DebugManager::GetInstance()->SetComponent("ParticleEmitter", name_, std::bind(&BaseParticleEmitter::DebugWindow, this));
#endif // _DEBUG
}

void BaseParticleEmitter::DebugWindow()
{
#ifdef _DEBUG

    ImGui::DragFloat("発生間隔", &emitterData_.emitInterval_, 0.02f, 0.1f, FLT_MAX);

    ImGui::InputInt("発生数", (int*)&emitterData_.emitNum_);
    ImGui::Checkbox("ランダム範囲生成", &emitterData_.enableRandomEmit_);
    if (emitterData_.enableRandomEmit_)
    {
        ImGui::DragFloat3("発生開始地点", &emitterData_.beginPosition_.x, 0.01f);
        ImGui::DragFloat3("発生終了地点", &emitterData_.endPosition_.x, 0.01f);
    }
    else
    {
        ImGui::DragFloat3("発生位置", &emitterData_.emitPositionFixed_.x, 0.01f);
    }

    ImGui::ColorEdit4("色", &emitterData_.color_.x);
    ImGui::Checkbox("ランダム速度", &emitterData_.enableRandomVelocity_);
    if (emitterData_.enableRandomVelocity_)
    {
        ImGui::DragFloat3("速度ランダム範囲開始", &emitterData_.velocityRandomRangeBegin_.x, 0.01f);
        ImGui::DragFloat3("速度ランダム範囲終了", &emitterData_.velocityRandomRangeEnd_.x, 0.01f);
    }
    else
    {
        ImGui::DragFloat3("速度固定", &emitterData_.velocityFixed_.x, 0.01f);
    }

    ImGui::DragFloat3("重力", &emitterData_.gravity_.x, 0.01f);
    ImGui::DragFloat3("抵抗", &emitterData_.resistance_.x, 0.01f);

#endif // _DEBUG
}