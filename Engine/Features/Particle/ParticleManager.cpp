#include "ParticleManager.h"

void ParticleManager::Update()
{
    for (auto& particle : particles_)
    {
        particle.Update();
    }
}

void ParticleManager::Draw()
{
    for (auto& particle : particles_)
    {
        particle.Draw();
    }
}

Particle& ParticleManager::CreateParticles()
{
    particles_.emplace_back();
    return particles_.back();
}
