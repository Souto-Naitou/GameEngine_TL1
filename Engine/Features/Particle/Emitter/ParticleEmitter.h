#pragma once

#include <Features/Particle/Emitter/BaseParticleEmitter.h>
#include <Features/Particle/Particle.h>
#include <Features/Primitive/AABB.h>
#include <memory>

class ParticleEmitter : public BaseParticleEmitter
{
public:
    ParticleEmitter() = default;
    ~ParticleEmitter() = default;

    void Initialize(const std::string& _filePath) override;
    void Update() override;
    void Draw();
    void Finalize() override;


private:
    void EmitParticle();


private:
    Particle* particle_ = nullptr;
    std::unique_ptr<AABB> aabb_;
};