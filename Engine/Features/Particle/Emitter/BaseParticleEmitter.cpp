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

    if (ImGui::CollapsingHeader("変形"))
    {
        ImGui::DragFloat3("開始スケール", &emitterData_.startScale_.x, 0.01f);
        ImGui::DragFloat3("終了スケール", &emitterData_.endScale_.x, 0.01f); ImGui::SameLine();
        if (ImGui::SmallButton("同期##Scale"))
        {
            emitterData_.endScale_ = emitterData_.startScale_;
        }
        ImGui::DragFloat("スケール遅延時間", &emitterData_.scaleDelayTime_, 0.01f);
    }

    if (ImGui::CollapsingHeader("一般"))
    {
        ImGui::DragFloat("パーティクル寿命", &emitterData_.particleLifeTime_, 0.1f, 0.0f, FLT_MAX);
        ImGui::DragFloat("発生間隔", &emitterData_.emitInterval_, 0.02f, 0.02f, FLT_MAX);
        ImGui::InputInt("発生数", (int*)&emitterData_.emitNum_);
        ImGui::ColorEdit4("色", &emitterData_.color_.x);
        ImGui::SliderFloat("透明度の変化量", &emitterData_.alphaDeltaValue_, -0.2f, 0.0f);
    }

    if (ImGui::CollapsingHeader("生成場所"))
    {
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
    }

    if (ImGui::CollapsingHeader("速度"))
    {
        ImGui::Checkbox("速度のランダマイズ", &emitterData_.enableRandomVelocity_);
        if (emitterData_.enableRandomVelocity_)
        {
            ImGui::DragFloat3("速度ランダム範囲-開始", &emitterData_.velocityRandomRangeBegin_.x, 0.01f);
            ImGui::DragFloat3("速度ランダム範囲-終了", &emitterData_.velocityRandomRangeEnd_.x, 0.01f);
        }
        else
        {
            ImGui::DragFloat3("速度", &emitterData_.velocityFixed_.x, 0.01f);
        }
    }

    if (ImGui::CollapsingHeader("物理"))
    {
        ImGui::DragFloat3("重力", &emitterData_.gravity_.x, 0.01f);
        ImGui::DragFloat3("抵抗", &emitterData_.resistance_.x, 0.01f);
    }

#endif // _DEBUG
}

void BaseParticleEmitter::Finalize()
{
#ifdef _DEBUG
    DebugManager::GetInstance()->DeleteComponent("ParticleEmitter", name_.c_str());
#endif
}