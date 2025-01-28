#include "TextSystem.h"

#include <Utility/ConvertString/ConvertString.h>
#include <cassert>

void TextSystem::Initialize()
{
    /// インスタンスの取得
    pDirectX12_ = DirectX12::GetInstance();
    d2dFactory_ = pDirectX12_->GetD2D1Factory();
    dxgiDevice_ = pDirectX12_->GetDXGIDevice();
    d2dDevice_ = pDirectX12_->GetDirect2dDevice();
    d2dDeviceContext_ = pDirectX12_->GetDirect2dDeviceContext();

    /// D2D1デバイスの生成
    this->CreateDirectWriteFactory();
}

IDWriteTextFormat* TextSystem::GetTextFormat(const std::string& _fontFamily, float _fontSize)
{
    std::pair<std::string, float> key = std::make_pair(_fontFamily, _fontSize);

    if (textFormats_.find(key) == textFormats_.end())
    {
        Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;

        
        HRESULT hr = dwriteFactory_->CreateTextFormat(
            ConvertString(_fontFamily).c_str(),
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            _fontSize,
            L"ja-jp",
            textFormat.GetAddressOf()
        );


        assert(SUCCEEDED(hr) && "テキストフォーマットの生成に失敗");

        textFormats_[key] = textFormat;
    }

    return textFormats_[key].Get();
}

void TextSystem::CreateDirectWriteFactory()
{
    /// DirectWriteのファクトリを生成
    hr_ = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory7), reinterpret_cast<IUnknown**>(dwriteFactory_.GetAddressOf()));
    assert(SUCCEEDED(hr_));
}
