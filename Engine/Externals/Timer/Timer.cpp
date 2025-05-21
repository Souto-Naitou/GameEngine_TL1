#include "./Timer.h"

Timer::Timer()
{
    QueryPerformanceFrequency(&mFreq_);
}

void Timer::Start()
{
    if (isRunning_) return;

    QueryPerformanceCounter(&mStart_);
    isStart_ = true;
    isRunning_ = true;
}

void Timer::Stop()
{
    if (!isRunning_) return;

    GetNow<double>();
    nowBeforeStop_ += now_;
    isRunning_ = false;
}

void Timer::Reset()
{
    mStart_ = {};
    now_ = 0.0;
    nowBeforeStop_ = 0.0;
    isStart_ = false;
    isRunning_ = false;
}