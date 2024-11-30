#pragma once

#include <chrono>
#include <Timer/Timer.h>

class FrameRate
{
public:
    FrameRate(const FrameRate&) = delete;
    FrameRate(const FrameRate&&) = delete;
    FrameRate& operator=(const FrameRate&) = delete;
    FrameRate& operator=(const FrameRate&&) = delete;

    static FrameRate* GetInstance() { static FrameRate instance; return &instance; }

    void    Initialize();
    void    FixFramerate();
    void    MeasureFPS();

    void    SetIntervalCalcurationFPS(double interval) { intervalCalcurationFPS_ = interval; }
    double  GetFPS() const { return fps_; }

private:
    FrameRate() = default;
    ~FrameRate() = default;
    std::chrono::steady_clock::time_point reference_;

    Timer timer_ = {};

    double          fps_                    = 0.0;
    unsigned int    frameCount_             = 0u;
    double          elapsedFrameCount_      = 0.0;
    double          intervalCalcurationFPS_ = 2.0;
};