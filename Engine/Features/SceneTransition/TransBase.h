#pragma once
#include <string>
#include <timer/Timer.h>


class TransBase
{
public:
    virtual ~TransBase() {};
    virtual void Initialize(const std::string& sceneName) = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;
    virtual void DebugWindow() = 0;

    bool IsEnd() const { return isEnd_; }

protected:
    Timer timer_;
    bool isEnd_ = false;
};