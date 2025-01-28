#include "Text.h"
#include <Utility/ConvertString/ConvertString.h>
#include <cassert>

void Text::Initialize()
{
    pDirectX12_ = DirectX12::GetInstance();
    pTextSystem_ = TextSystem::GetInstance();
    dwriteFactory_ = pTextSystem_->GetDWriteFactory();
}

void Text::CreateTextLayout(const std::string& _text, const std::string& _fontFamily, float _fontSize, float _x, float _y, float _width, float _height)
{
    std::wstring w_text = ConvertString(_text);

    auto* textFormat = pTextSystem_->GetTextFormat(_fontFamily, _fontSize);

    HRESULT hr = dwriteFactory_->CreateTextLayout(
        w_text.c_str(),
        static_cast<uint32_t>(w_text.size()),
        textFormat,
        _width,
        _height,
        &textLayout_
    );

    assert(SUCCEEDED(hr) && "テキストレイアウトの生成に失敗");
}
