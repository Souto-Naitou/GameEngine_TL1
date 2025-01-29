#pragma once

#include <Core/DirectX12/DirectX12.h>

#include "TextSystem.h"

class Text
{
public:
    Text() = default;
    ~Text() = default;

    void Initialize();
    void Update();
    void Draw();
    void Finalize();

public: /// Setter
    void SetName(const std::string& _name) { name_ = _name; }
    void SetText(const std::string& _text) { text_ = _text; isChanged_ = true; }
    void SetPosition(const Vector2& _pos) { position_ = _pos; isChanged_ = true; }
    void SetMaxSize(const Vector2& _maxsize) { maxsize_ = _maxsize; isChanged_ = true; }
    void SetFontFamily(const std::string& _fontFamily) { fontFamily_ = _fontFamily; isChanged_ = true; }
    void SetFontSize(float _fontSize) { fontSize_ = _fontSize; isChanged_ = true;}
    void SetColorName(const std::string& _key) { keyColor_ = _key; }


    void CreateTextLayout();


private:
    std::string text_;
    bool isChanged_ = false;

    std::string fontFamily_;
    bool isVisibleRange_ = false;

    Vector2 position_;
    Vector2 maxsize_;
    float fontSize_ = 0.0f;

    std::string keyColor_ = "Black";

    Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout_;
    IDWriteTextFormat* textFormat_ = nullptr;

private: /// 借り物
    DirectX12* pDirectX12_ = nullptr;
    IDWriteFactory7* dwriteFactory_ = nullptr;
    TextSystem* pTextSystem_ = nullptr;
    ID2D1DeviceContext2* d2dDeviceContext_ = nullptr;


private:
    void DebugWindow();
    std::string name_;

};