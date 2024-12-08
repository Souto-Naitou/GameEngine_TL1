#pragma once

#include <vector>
#include <Features/Particle/Particle.h>

class ParticleManager
{
public:
    ParticleManager(ParticleManager const&) = delete;
    void operator=(ParticleManager const&) = delete;
    ParticleManager(ParticleManager const&&) = delete;
    void operator=(ParticleManager const&&) = delete;

    static ParticleManager* GetInstance()
    {
        static ParticleManager instance;
        return &instance;
    }


public:
    void Update();
    void Draw();


public:
    Particle& CreateParticle();
    void ReleaseParticle(Particle* _particle);

private:
    ParticleManager() = default;
    ~ParticleManager() = default;


public:
    std::vector<Particle> particles_;

};