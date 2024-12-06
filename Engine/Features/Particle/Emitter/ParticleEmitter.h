#pragma once

#include <Features/Particle/Emitter/BaseParticleEmitter.h>

class ParticleEmitter : public BaseParticleEmitter
{
public:
    void Initialize() override;
    void Update() override;

private:
    void EmitParticle();
};