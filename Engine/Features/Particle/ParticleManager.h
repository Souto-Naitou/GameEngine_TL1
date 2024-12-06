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

    void CreateParticles();

private:
    ParticleManager() = default;
    ~ParticleManager() = default;


public:
    std::vector<Particle> particles_;

};