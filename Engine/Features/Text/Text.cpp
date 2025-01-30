#include "Text.h"
#include <Utility/ConvertString/ConvertString.h>
#include <cassert>

#include <DebugTools/DebugManager/DebugManager.h>
#include <imgui.h>

void Text::Initialize()
{
    pDirectX12_ = DirectX12::GetInstance();
    pTextSystem_ = TextSystem::GetInstance();
    dwriteFactory_ = pTextSystem_->GetDWriteFactory();
    d2dDeviceContext_ = pTextSystem_->GetD2D1DeviceContext();

    DebugManager::GetInstance()->SetComponent("Text", name_, BINDCOMPONENT(Text, DebugWindow));

    fontFamily_ = "Bahnschrift";
}

void Text::Update()
{
    CreateTextLayout();
}

void Text::Draw()
{
    const auto solidColorBrush = pTextSystem_->GetColorBrush(keyColor_);
    d2dDeviceContext_->DrawTextLayout(
        D2D1::Point2F(position_.x, position_.y),
        textLayout_.Get(),
        solidColorBrush
    );
}

void Text::Finalize()
{
    DebugManager::GetInstance()->DeleteComponent("Text", name_.c_str());
}

void Text::CreateTextLayout()
{
    std::wstring w_text = ConvertString(text_);

    textFormat_ = pTextSystem_->GetTextFormat(fontFamily_, fontSize_);

    HRESULT hr = dwriteFactory_->CreateTextLayout(
        w_text.c_str(),
        static_cast<uint32_t>(w_text.size()),
        textFormat_,
        maxsize_.x,
        maxsize_.y,
        &textLayout_
    );

    assert(SUCCEEDED(hr) && "テキストレイアウトの生成に失敗");

    return;
}

void Text::DebugWindow()
{
    char buffer[256] = { 0 };
    strcpy_s(buffer, text_.c_str());

    ImGui::DragFloat2("Position", &position_.x);
    ImGui::Checkbox("Visible Size", &isVisibleRange_);
    ImGui::DragFloat2("MaxSize", &maxsize_.x);

    if (ImGui::InputText("Text", buffer, IM_ARRAYSIZE(buffer)))
    {
        text_ = buffer;
    }

    int fontSize = static_cast<int>(fontSize_);

    if (ImGui::InputInt("Font Size", &fontSize))
    {
        fontSize_ = static_cast<float>(fontSize);
    }

    strcpy_s(buffer, fontFamily_.c_str());
    if (ImGui::InputText("Font Family", buffer, IM_ARRAYSIZE(buffer)))
    {
        fontFamily_ = buffer;
    }
}
