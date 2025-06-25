#include "ParticleManager.h"
#include <cassert>

void ParticleManager::Update()
{
    for (auto& particle : particles_)
    {
        particle->Update();
    }

    deleteParticles_.remove_if([&](Particle* _particle)
    {
        if (_particle->IsAbleDelete())
        {
            ReleaseParticle(_particle);
            return true;
        }
        return false;
    });
}

void ParticleManager::Draw()
{
    for (auto& particle : particles_)
    {
        particle->Draw();
    }
}

void ParticleManager::Finalize()
{
    ReleaseAllParticle();
}

Particle* ParticleManager::CreateParticle()
{
    auto& ref = particles_.emplace_back(std::make_unique<Particle>());
    ref->SetDirectX12(pDx12_);
    return ref.get();
}

void ParticleManager::ReleaseParticle(Particle* _particle)
{
    for (auto itr = particles_.begin(); itr != particles_.end(); ++itr)
    {
        auto ptr = itr->get();
        if (ptr == _particle)
        {
            ptr->Finalize();
            particles_.erase(itr);
            return;
        }
    }

    assert(false);

    return;
}

void ParticleManager::ReserveDeleteParticle(Particle* _particle)
{
    deleteParticles_.push_back(_particle);
    return;
}

void ParticleManager::ReleaseAllParticle()
{
    for (auto& particle : particles_)
    {
        particle->Finalize();
    }

    particles_.clear();
    deleteParticles_.clear();
    return;
}
