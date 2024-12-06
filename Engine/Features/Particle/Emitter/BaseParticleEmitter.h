#pragma once

#include <Timer/Timer.h>

class BaseParticleEmitter
{
public:
    virtual void Initialize() = 0;
    virtual void Update() = 0;

protected:
    Timer timer_;
    float emitInterval_;
    float lifeTime_;
    Vector3 beginPosition_;
    Vector3 endPosition_;
};