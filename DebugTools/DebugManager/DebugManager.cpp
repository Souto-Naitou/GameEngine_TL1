#include "DebugTools/DebugManager/DebugManager.h"


#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_dx12.h>
#endif // _DEBUG

DebugManager::DebugManager()
{
#ifdef _DEBUG
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
    //ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, 0.75f));
    //ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.8f, 0.1f, 0.1f, 0.75f));
    //ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.0f, 0.0f, 0.90f));
#endif // _DEBUG
}

DebugManager::~DebugManager()
{

}

void DebugManager::DebugWindowOverall()
{
#ifdef _DEBUG
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    const float PAD = 10.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = PAD;
    window_pos.y = PAD;
    window_pos_pivot.x = 0.0f;
    window_pos_pivot.y = 0.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowViewport(viewport->ID);
    window_flags |= ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowBgAlpha(0.0f);

    if (ImGui::Begin("Overray FPS", nullptr, window_flags))
    {
        ImGui::Text("%.2lfFPS", fps_);
        ImGui::SameLine();
        ImGui::ProgressBar(static_cast<float>(fps_) / 60.0f, ImVec2(0, 0), "");
    }
    ImGui::End();
#endif // _DEBUG
}

void DebugManager::MeasureFPS()
{
    if (!timer_.GetIsStart())
    {
        timer_.Start();
    }
    /// フレームレート計算
    if (timer_.GetNow() - elapsedFrameCount_ >= 2.0)
    {
        fps_ = frameCount_ * 1.0 / (timer_.GetNow() - elapsedFrameCount_);

        frameCount_ = 0;
        elapsedFrameCount_ = timer_.GetNow();
    }
    frameCount_++;
}

void DebugManager::Window_ObjectList()
{
#ifdef _DEBUG
    ImGui::PushID("WindowObjectList");
    if (ImGui::Begin("Objects"))
    {
        for (auto itr = componentList_.begin(); itr != componentList_.end();)
        {
            if (std::get<0>(*itr) == "null-name")
            {
                ImGui::Selectable(std::get<1>(*itr).c_str(), &std::get<3>(*itr));
                itr++;
            }
            else // null-nameじゃないとき
            {
                std::string parentName = std::get<0>(*itr).c_str();
                if (ImGui::TreeNode(parentName.c_str()))
                {
                    do
                    {
                        //parentName = std::get<0>(*itr);
                        ImGui::Selectable(std::get<1>(*itr).c_str(), &std::get<3>(*itr));
                        itr++;
                        if (itr == componentList_.end()) break;
                    } while (std::get<0>(*itr) == parentName);

                    ImGui::TreePop();
                }
                else
                {
                    while (std::get<0>(*itr) == parentName)
                    {
                        itr++;
                        if (itr == componentList_.end()) break;
                    }
                }
            }
        }
    }
    ImGui::End();
    ImGui::PopID();
#endif // DEBUG
}

std::list<std::tuple<std::string, std::string, const std::function<void(void)>, bool>>::iterator
    DebugManager::GetInsertIterator(std::string _parentName)
{
    auto resultItr = std::find_if(
        componentList_.begin(),
        componentList_.end(),
        [_parentName](std::tuple<std::string, std::string, const std::function<void()>, bool> arg) {
        if (std::get<0>(arg).compare(_parentName) == 0)
        {
            return true;
        }
        return false;
    });

    if (resultItr == componentList_.end())
    {
        //return std::prev(resultItr);
        return componentList_.begin();
    }

    do
    {
        resultItr++;
        if (resultItr == componentList_.end())
        {
            return std::prev(resultItr);
        }
    } while (std::get<0>(*resultItr) == _parentName);

    return resultItr;
}

void DebugManager::DeleteComponent(const char* _strID)
{
    componentList_.remove_if([_strID](const auto& component) {
        return std::get<0>(component).compare("null-name") == 0 &&
            std::get<1>(component).compare(_strID) == 0;
    });
}

void DebugManager::DeleteComponent(const char* _parentID, const char* _childID)
{
    componentList_.remove_if([_parentID, _childID](const auto& component) {
        return std::get<0>(component).compare(_parentID) == 0 &&
            std::get<1>(component).compare(_childID) == 0;
    });
}

//void DebugManager::DeleteComponent(const void* _ptr)
//{
//
//}

void DebugManager::DrawUI()
{
#ifdef _DEBUG

    if (!onDisplay_) return;

    MeasureFPS();

    DebugWindowOverall();

    // 登録されていないなら早期リターン
    if (componentList_.size() == 0) return;

    ImGui::PushID("DEBUG_MANAGER");

    Window_ObjectList();

    // デバッグウィンドウ描画
    ImGui::Begin("デバッグ");

    ImGuiTabBarFlags flag = {};
    flag |= ImGuiTabBarFlags_Reorderable;
    flag |= ImGuiTabBarFlags_FittingPolicyResizeDown;
    flag |= ImGuiTabBarFlags_TabListPopupButton;

    ImGui::BeginTabBar("## TABBAR", flag);
    for (auto& component : componentList_)
    {
        // componentを展開 (参照)
        auto& [parentID, childID, pFunc, enableTab] = component;

        if (enableTab)
        {
            std::string tabName;
            if (parentID.compare("null-name") == 0) tabName = childID;
            else tabName = parentID + childID;

            std::string id_str = tabName + "TABITEM";
            ImGui::PushID(id_str.c_str());
            if (enableTab && parentID.compare("#Window") == 0 && ImGui::Begin(childID.c_str(), &enableTab))
            {
                pFunc();
                ImGui::End();
            }
            else if (enableTab && ImGui::BeginTabItem(tabName.c_str(), &enableTab))
            {
                pFunc();
                ImGui::EndTabItem();
            }
            ImGui::PopID();
        }
    }
    ImGui::EndTabBar();


    ImGui::End();

    ImGui::PopID();
#endif // _DEBUG
}

void DebugManager::ChangeFont()
{
#ifdef _DEBUG
    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig fontcfg;
    fontcfg.MergeMode = 0;
    fontcfg.OversampleH = 1;
    fontcfg.PixelSnapH = 1;
    fontcfg.GlyphOffset = ImVec2(0.0f, 0.0f);


    ImFont* resultFont = io.Fonts->AddFontFromFileTTF(
        "Resources/Font/NotoSansCJKjp-Light.ttf",
        13,
        &fontcfg,
        io.Fonts->GetGlyphRangesJapanese()
    );

    io.FontDefault = resultFont;

    io.Fonts->Build();
    ImGui_ImplDX12_CreateDeviceObjects();
#endif // _DEBUG
}
