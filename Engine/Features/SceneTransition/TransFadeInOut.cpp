#include "TransFadeInOut.h"
#include <Core/DirectX12/TextureManager.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <DebugTools/ImGuiTemplates/ImGuiTemplates.h>
#include <Features/SceneManager/SceneManager.h>
#include <Core/Win32/WinSystem.h>

void TransFadeInOut::Initialize(const std::string& _sceneName)
{
    sceneName_ = _sceneName;

    screenWidth_ = WinSystem::kClientWidth;
    screenHeight_ = WinSystem::kClientHeight;

    easing_ = std::make_unique<Easing>("Trans_FadeInOut");

    sprite_ = std::make_unique<Sprite>();
    sprite_->Initialize("white32x32.png");
    sprite_->SetColor({ 0,0,0,0 });
    sprite_->SetSize({ screenWidth_, screenHeight_ });
    timer_.Start();
    easing_->Start();
    DebugManager::GetInstance()->SetComponent(
        "Transition", "FadeInOut", std::bind(&TransFadeInOut::DebugWindow, this));
}

void TransFadeInOut::Update()
{
    if (countPhase_ == 2)
    {
        isEnd_ = true;
        return;
    }
    if (easing_->GetIsEnd())
    {
        easing_->Reset();
        easing_->Start();
        countPhase_++;
    }
    if (!isChangedScene_ && countPhase_ == 1)
    {
        isChangedScene_ = true;
        SceneManager::GetInstance()->ReserveScene(sceneName_);
    }

    if (countPhase_ == 0)
    {
        opacity_ = easing_->Update();
    }
    else if (countPhase_ == 1)
    {
        opacity_ = 1.0f - easing_->Update();
    }

    sprite_->SetColor(Vector4(0, 0, 0, opacity_));
    sprite_->Update();
}

void TransFadeInOut::Draw()
{
    sprite_->Draw();
}

void TransFadeInOut::Finalize()
{
    DebugManager::GetInstance()->DeleteComponent("Transition", "FadeInOut");
}

void TransFadeInOut::DebugWindow()
{
#ifdef _DEBUG

    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("Scene Name", sceneName_);
        ImGuiTemplate::VariableTableRow("Timer", timer_.GetNow());
        ImGuiTemplate::VariableTableRow("Phase", countPhase_);
        ImGuiTemplate::VariableTableRow("Opacity", opacity_);
    };

    ImGuiTemplate::VariableTable("SCT_FadeInOut", pFunc);

#endif
}

TransFadeInOut::~TransFadeInOut()
{
    OutputDebugStringA("deleted TransFadeInOut\n");
}
