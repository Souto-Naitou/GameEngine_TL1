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

    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("発生間隔", emitterData_.emitInterval_);
        ImGuiTemplate::VariableTableRow("エミッター生存時間", emitterData_.emitterLifeTime_);
        if (emitterData_.enableRandomEmit_)
        {
            ImGuiTemplate::VariableTableRow("発生開始地点", emitterData_.beginPosition_);
            ImGuiTemplate::VariableTableRow("発生終了地点", emitterData_.endPosition_);
        }
        else
        {
            ImGuiTemplate::VariableTableRow("発生位置", emitterData_.emitPositionFixed_);
        }
        ImGuiTemplate::VariableTableRow("ランダム範囲生成の有無", emitterData_.enableRandomEmit_);
    };

    ImGui::DragFloat("発生間隔", &emitterData_.emitInterval_, 0.01f);
    if (emitterData_.enableRandomEmit_)
    {
        ImGui::DragFloat3("発生開始地点", &emitterData_.beginPosition_.x, 0.01f);
        ImGui::DragFloat3("発生終了地点", &emitterData_.endPosition_.x, 0.01f);
    }
    else
    {
        ImGui::DragFloat3("発生位置", &emitterData_.emitPositionFixed_.x, 0.01f);
    }

    ImGuiTemplate::VariableTable("EmitterData", pFunc);

#endif // _DEBUG
}