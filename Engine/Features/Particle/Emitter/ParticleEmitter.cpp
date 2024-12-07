#include "ParticleEmitter.h"

#include <Features/Particle/ParticleManager.h>
#include <Features/RandomGenerator/RandomGenerator.h>

void ParticleEmitter::Initialize(const std::string& _filePath)
{
    BaseParticleEmitter::Initialize(_filePath);

    timer_.Start();
    particle_ = &ParticleManager::GetInstance()->CreateParticles();
    particle_->Initialize(_filePath);
    particle_->reserve(100);
    emitterData_.emitPositionFixed_ = Vector3(0.0f, 0.0f, 0.0f);
    emitterData_.emitInterval_ = 0.1f;
    emitterData_.emitterLifeTime_ = 0.0f;
}

void ParticleEmitter::Update()
{
    if (timer_.GetNow() > emitterData_.emitInterval_)
    {
        EmitParticle();
        timer_.Reset();
        timer_.Start();
    }
}

void ParticleEmitter::EmitParticle()
{
    RandomGenerator* random = RandomGenerator::GetInstance();

    particle_->emplace_back({});
    auto& parameter = particle_->GetParticleData();
    auto& datum = parameter.back();

    // 初期トランスフォーム
    datum.transform_.translate = emitterData_.emitPositionFixed_;
    datum.transform_.scale = Vector3(0.3f, 0.3f, 0.3f);

    // ライフタイム
    datum.lifeTime_ = emitterData_.emitterLifeTime_;

    // 速度
    datum.velocity_ = Vector3(random->Generate(-1.0f, 1.0f), random->Generate(-1.0f, 1.0f), random->Generate(-1.0f, 1.0f));
    // 初期の色
    datum.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
    // アルファ値の変化量
    datum.alphaDeltaValue_ = -0.01f;
    // 消去条件
    datum.deleteCondition_ = ParticleDeleteCondition::ZeroAlpha;
}
