#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <wrl.h>
#include <cstdint>

class Input
{
public:
    enum class MouseNum
    {
        Left = 0,
        Right = 1,
        Center = 2,
    };

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;
    Input(Input&&) = delete;
    Input& operator=(Input&&) = delete;

    static Input* GetInstance()
    {
        static Input instance;
        return &instance;
    }

    void        Initialize(HINSTANCE _hInstance, HWND _hwnd);
    void        Update();

    void        Enable(bool _flag);

    bool        PushKey(BYTE _keyNumber) const;
    bool        PushKeyC(char _key) const;
    bool        TriggerKey(BYTE _keyNumber) const;
    bool        TriggerKeyC(char _key) const;

    bool        PushMouse(MouseNum _mouseNum) const;
    bool        TriggerMouse(MouseNum _mouseNum) const;
    int32_t     GetWheelDelta() const;


private:
    Input() = default;
    ~Input() = default;
    
    // Internal functions
    BYTE GetKeyNumber(char _key) const;
    void MapInputData();

    // State
    bool isEnable_ = true;

    // DirectInput data
    Microsoft::WRL::ComPtr<IDirectInput8> directInput_ = nullptr;
    Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
    Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse_ = nullptr;
    DIMOUSESTATE2 mouseState_ = {};

    // Logical input data
    BYTE    key_[256]           = {};
    BYTE    keyPre_[256]        = {};
    bool    leftClick_          = false;
    bool    leftClickPre_       = false;
    bool    rightClick_         = false;
    bool    rightClickPre_      = false;
    int32_t wheelDelta_         = 0;
};