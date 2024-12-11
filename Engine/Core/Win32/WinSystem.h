#pragma once
#include <Windows.h>
#include <stdint.h>

class WinSystem
{
public:
    static const uint32_t kClientWidth = 1600u;
    static const uint32_t kClientHeight = 900u;

    WinSystem(const WinSystem&) = delete;
    WinSystem(const WinSystem&&) = delete;
    WinSystem& operator=(const WinSystem&) = delete;
    WinSystem& operator=(const WinSystem&&) = delete;

    static WinSystem* GetInstance() { static WinSystem instance;return &instance; };

    HWND GetHwnd() const { return hwnd_; }
    HINSTANCE GetHinstance() const { return wc_.hInstance; }

    void Initialize();
    void Finalize();
    void ShowWnd();
    UINT GetMsg();

private:
    WinSystem() = default;
    ~WinSystem() = default;

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    WNDCLASS    wc_         = {};
    HWND        hwnd_       = {};
    MSG         msg_        = {};
    static bool isMoving_;
};