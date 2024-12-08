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

Particle& ParticleManager::CreateParticle()
{
    particles_.emplace_back();
    return particles_.back();
}

void ParticleManager::ReleaseParticle(Particle* _particle)
{
    for (auto itr = particles_.begin(); itr != particles_.end(); ++itr)
    {
        if (&(*itr) == _particle)
        {
            particles_.erase(itr);
            return;
        }
    }

    assert(false);

    return;
}
