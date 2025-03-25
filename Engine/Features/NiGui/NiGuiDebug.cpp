#include "NiGuiDebug.h"

#include <imgui.h>

void NiGuiDebug::DrawDebugUI()
{
    ImGuiTreeNodeFlags parentFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

    bool isOpen = ImGui::Begin("NiUI_DebugInfo");


    if (!isOpen)
    {
        ImGui::End();
        return;
    }

    if (ImGui::TreeNodeEx("Setting", parentFlags))
    {
        if (ImGui::TreeNode("Buffer Setting"))
        {
            ImGui::InputInt("Delete Element Threshold", &(setting_->deleteElementThreshold));
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("State", parentFlags))
    {
        if (ImGui::TreeNode("Validation"))
        {
            ImGui::Text("isInitialized : %s", state_->valid.isInitialized ? "true" : "false");
            ImGui::Text("isBeginFrame : %s", state_->valid.isBeginFrame ? "true" : "false");
            ImGui::Text("nestCount : %d", state_->valid.nestCount);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("ComponentID"))
        {
            ImGui::Text("TypeHover : %s", state_->componentID.typeHover.c_str());
            ImGui::Text("TypeActive : %s", state_->componentID.typeActive.c_str());
            ImGui::Text("ActiveComponentID : %s", state_->componentID.active.c_str());
            ImGui::Text("HoverComponentID : %s", state_->componentID.hover.c_str());
            ImGui::Text("PreHoverComponentID : %s", state_->componentID.preHover.c_str());
            ImGui::Text("PreActiveComponentID : %s", state_->componentID.preActive.c_str());
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Time"))
        {
            ImGui::Text("HoverTime : %d", state_->time.hover);
            ImGui::Text("ActiveTime : %d", state_->time.active);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("IO", parentFlags))
    {
        if (ImGui::TreeNode("InputData"))
        {
            ImGui::Text("CursorPos : (%.2f, %.2f)", io_->input.cursorPos.x, io_->input.cursorPos.y);
            ImGui::Text("TriggeredPos : (%.2f, %.2f)", io_->input.triggeredPos.x, io_->input.triggeredPos.y);
            ImGui::Text("DifferencePos : (%.2f, %.2f)", io_->input.differencePos.x, io_->input.differencePos.y);
            ImGui::Text("isLeft : %s", io_->input.isLeft ? "true" : "false");
            ImGui::Text("isLeftPre : %s", io_->input.isLeftPre ? "true" : "false");
            ImGui::Text("isRight : %s", io_->input.isRight ? "true" : "false");
            ImGui::Text("isRightPre : %s", io_->input.isRightPre ? "true" : "false");
            ImGui::Text("isMiddle : %s", io_->input.isMiddle ? "true" : "false");
            ImGui::Text("isMiddlePre : %s", io_->input.isMiddlePre ? "true" : "false");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("AudioHandle"))
        {
            ImGui::Text("ButtonHover : %d", io_->audioHnd.buttonHover);
            ImGui::Text("ButtonConfirm : %d", io_->audioHnd.buttonConfirm);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    ImGui::End();
}
