#include "Framerate.h"
#include <thread>

#include <Windows.h>
#pragma comment(lib, "Winmm.lib")

void FrameRate::Initialize()
{
    timeBeginPeriod(1);
    reference_ = std::chrono::steady_clock::now();
}

void FrameRate::FixFramerate()
{
    const std::chrono::microseconds kMinTime(static_cast<uint64_t>(1000000.0f / 60.0f));
    const std::chrono::microseconds kMinCheckTime(static_cast<uint64_t>(1000000.0f / 65.0f));

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

    if (elapsed < kMinCheckTime)
    {
        while (std::chrono::steady_clock::now() - reference_ < kMinTime)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }

    reference_ = std::chrono::steady_clock::now();
}
