#include "Input.h"
#include "Input.h"

#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void Input::Initialize(HINSTANCE _hInstance, HWND _hwnd)
{
    HRESULT hr = DirectInput8Create(
        _hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr
    );
    assert(SUCCEEDED(hr));

    // キーボードデバイスの生成
    hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
    assert(SUCCEEDED(hr));

    // 入力データ形式のセット
    hr = keyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(hr));

    // 排他制御レベルのセット
    hr = keyboard_->SetCooperativeLevel(
        _hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
    );
    assert(SUCCEEDED(hr));

    // マウスデバイスの生成
    hr = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
    assert(SUCCEEDED(hr));

    // 入力データ形式のセット
    hr = mouse_->SetDataFormat(&c_dfDIMouse);
    assert(SUCCEEDED(hr));

    // 排他制御レベルのセット
    hr = mouse_->SetCooperativeLevel(
        _hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
    );
    assert(SUCCEEDED(hr));
}

void Input::Update()
{
    // キーボード情報の取得開始
    keyboard_->Acquire();

    // 配列をコピー
    memcpy(keyPre_, key_, 256);

    // 全キーの入力情報を取得
    keyboard_->GetDeviceState(sizeof(key_), key_);

    // マウス情報の取得開始
    mouse_->Acquire();

    // マウスの入力情報を取得
    mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);

    // 左クリックの入力情報を取得
    leftClickPre_ = leftClick_;
    if (mouseState_.rgbButtons[0] & 0x80)
    {
        leftClick_ = true;
    }
    else
    {
        leftClick_ = false;
    }

    // 右クリックの入力情報を取得
    rightClickPre_ = rightClick_;
    if (mouseState_.rgbButtons[1] & 0x80)
    {
        rightClick_ = true;
    }
    else
    {
        rightClick_ = false;
    }
}

bool Input::PushKey(BYTE _keyNumber) const
{
    // 指定キーを押して入ればtrueを返す
    if (key_[_keyNumber])
    {
        return true;
    }
    return false;
}

bool Input::PushKeyC(char _key) const
{
    // 指定キーを押して入ればtrueを返す
    if(key_[GetKeyNumber(_key)])
    {
        return true;
    }

    return false;
}

bool Input::TriggerKey(BYTE _keyNumber) const
{
    // キーが押された瞬間ならtrueを返す
    if(key_[_keyNumber] && !keyPre_[_keyNumber])
    {
        return true;
    }

    return false;
}

bool Input::TriggerKeyC(char _key) const
{
    // キーが押された瞬間ならtrueを返す
    BYTE keyNumber = GetKeyNumber(_key);
    if(key_[keyNumber] && !keyPre_[keyNumber])
    {
        return true;
    }

    return false;
}

bool Input::PushMouse(MouseNum _mouseNum) const
{
    if (_mouseNum == MouseNum::Left)
    {
        if (leftClick_)
        {
            return true;
        }
    }
    else if (_mouseNum == MouseNum::Right)
    {
        if (rightClick_)
        {
            return true;
        }
    }

    return false;
}

bool Input::TriggerMouse(MouseNum _mouseNum) const
{
    if (_mouseNum == MouseNum::Left)
    {
        if (leftClick_ && !leftClickPre_)
        {
            return true;
        }
    }
    else if (_mouseNum == MouseNum::Right)
    {
        if (rightClick_ && !rightClickPre_)
        {
            return true;
        }
    }

    return false;
}

BYTE Input::GetKeyNumber(char _key) const
{
    // キーの文字からキー番号を取得
    return MapVirtualKey(_key, MAPVK_VK_TO_VSC);
}
