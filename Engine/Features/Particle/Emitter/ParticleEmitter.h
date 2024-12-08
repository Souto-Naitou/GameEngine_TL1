#pragma once

#include <Features/Particle/Emitter/BaseParticleEmitter.h>
#include <Features/Particle/Particle.h>

class ParticleEmitter : public BaseParticleEmitter
{
public:
    ParticleEmitter() = default;
    ~ParticleEmitter() = default;

    void Initialize(const std::string& _filePath) override;
    void Update() override;
    void Finalize() override;


private:
    void EmitParticle();


private:
    Particle* particle_ = nullptr;
};