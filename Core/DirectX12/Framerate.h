#pragma once

#include <chrono>

class FrameRate
{
public:
    FrameRate(const FrameRate&) = delete;
    FrameRate(const FrameRate&&) = delete;
    FrameRate& operator=(const FrameRate&) = delete;
    FrameRate& operator=(const FrameRate&&) = delete;

    static FrameRate* GetInstance() { static FrameRate instance; return &instance; }

    void Initialize();
    void FixFramerate();

private:
    FrameRate() = default;
    ~FrameRate() = default;

    std::chrono::steady_clock::time_point reference_;

};