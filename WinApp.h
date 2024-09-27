#pragma once
#include <Windows.h>
#include <stdint.h>

class WinApp
{
public:
    static const uint32_t kClientWidth = 1280u;
    static const uint32_t kClientHeight = 720u;

    WinApp(const WinApp&) = delete;
    WinApp(const WinApp&&) = delete;
    WinApp& operator=(const WinApp&) = delete;
    WinApp& operator=(const WinApp&&) = delete;

    static WinApp* GetInstance() { static WinApp instance;return &instance; };

    HWND GetHwnd() const { return hwnd_; }

    void Initialize();
    void Finalize();
    void ShowWnd();
    UINT GetMsg();

private:
    WinApp() = default;
    ~WinApp() = default;

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    WNDCLASS    wc_         = {};
    HWND        hwnd_       = {};
    MSG         msg_        = {};
};