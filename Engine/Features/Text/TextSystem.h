#pragma once

#include <Core/DirectX12/DirectX12.h>

#include <wrl.h>
#include <dwrite.h>
#include <map>
#include <string>
#include <utility>

class TextSystem
{
public:
    TextSystem(const TextSystem&) = delete;
    TextSystem(const TextSystem&&) = delete;
    TextSystem& operator=(const TextSystem&) = delete;
    TextSystem& operator=(const TextSystem&&) = delete;

    static TextSystem* GetInstance()
    {
        static TextSystem instance; return &instance;
    }
    
    void Initialize();
    void PresentDraw();
    void PostDraw();


public: /// Getter
    IDWriteFactory7* GetDWriteFactory() const { return dwriteFactory_.Get(); }
    IDWriteTextFormat* GetTextFormat(const std::string& _fontFamily, float _fontSize);


private:
    TextSystem() = default;
    ~TextSystem() = default;

    void CreateDirectWriteFactory();

private:
    HRESULT hr_ = 0;

    std::map<std::pair<std::string, float>, Microsoft::WRL::ComPtr<IDWriteTextFormat>> textFormats_;

    Microsoft::WRL::ComPtr<IDWriteFactory7> dwriteFactory_ = nullptr;      // DirectWrite


private: /// 借り物
    DirectX12* pDirectX12_ = nullptr;
    IDXGIDevice* dxgiDevice_ = nullptr;
    ID2D1Factory3* d2dFactory_ = nullptr;
    ID2D1Device2* d2dDevice_ = nullptr;
    ID2D1DeviceContext2* d2dDeviceContext_ = nullptr;
};