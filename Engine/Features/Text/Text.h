#pragma once

#include <Core/DirectX12/DirectX12.h>

#include "TextSystem.h"

class Text
{
public:
    Text() = default;
    ~Text() = default;

    void Initialize();
    void PresentDraw();
    void PostDraw();

    void CreateTextLayout(const std::string& _text, const std::string& _fontFamily, float _fontSize, float _x, float _y, float _width, float _height);


private:
    Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout_;

private: /// 借り物
    DirectX12* pDirectX12_ = nullptr;
    IDWriteFactory7* dwriteFactory_ = nullptr;
    TextSystem* pTextSystem_ = nullptr;
};