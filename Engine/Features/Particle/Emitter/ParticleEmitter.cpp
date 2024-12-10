#include "ParticleEmitter.h"

#include <Features/Particle/ParticleManager.h>
#include <Features/RandomGenerator/RandomGenerator.h>

void ParticleEmitter::Initialize(const std::string& _filePath)
{
    BaseParticleEmitter::Initialize(_filePath);

    timer_.Start();
    particle_ = &ParticleManager::GetInstance()->CreateParticle();
    particle_->Initialize(_filePath);
    particle_->reserve(6000);
    emitterData_.emitPositionFixed_ = Vector3(0.0f, 0.0f, 0.0f);
    emitterData_.emitInterval_ = 0.2f;
    emitterData_.emitterLifeTime_ = 0.0f;
    emitterData_.emitNum_ = 1;
    aabb_ = std::make_unique<AABB>();
    aabb_->Initialize();
}

void ParticleEmitter::Update()
{
    if (timer_.GetNow() > emitterData_.emitInterval_)
    {
        if (emitterData_.emitNum_ < 0)
        {
            emitterData_.emitNum_ = 0;
        }
        for (int32_t i = 0; i < emitterData_.emitNum_; ++i)
        {
            EmitParticle();
        }
        timer_.Reset();
        timer_.Start();
    }
}

void ParticleEmitter::Draw()
{
    aabb_->Draw();
}

void ParticleEmitter::Finalize()
{
    BaseParticleEmitter::Finalize();

    particle_->Finalize();
    ParticleManager::GetInstance()->ReleaseParticle(particle_);
    particle_ = nullptr;
}

void ParticleEmitter::EmitParticle()
{
    RandomGenerator* random = RandomGenerator::GetInstance();

    particle_->emplace_back({});
    auto& parameter = particle_->GetParticleData();
    auto& datum = parameter.back();

    /// 初期トランスフォーム
    if (emitterData_.enableRandomEmit_)
    {
        datum.transform_.translate = Vector3(
            random->Generate(emitterData_.beginPosition_.x, emitterData_.endPosition_.x),
            random->Generate(emitterData_.beginPosition_.y, emitterData_.endPosition_.y),
            random->Generate(emitterData_.beginPosition_.z, emitterData_.endPosition_.z));
    }
    else
    {
        datum.transform_.translate = emitterData_.emitPositionFixed_;
    }

    /// スケール
    datum.transform_.scale = emitterData_.startScale_;
    datum.startScale_ = emitterData_.startScale_;
    datum.endScale_ = emitterData_.endScale_;
    datum.scaleDelayTime_ = emitterData_.scaleDelayTime_;

    /// ライフタイム
    datum.lifeTime_ = emitterData_.particleLifeTime_;

    /// 速度
    if (emitterData_.enableRandomVelocity_)
    {
        datum.velocity_ = Vector3(
            random->Generate(emitterData_.velocityRandomRangeBegin_.x, emitterData_.velocityRandomRangeEnd_.x),
            random->Generate(emitterData_.velocityRandomRangeBegin_.y, emitterData_.velocityRandomRangeEnd_.y),
            random->Generate(emitterData_.velocityRandomRangeBegin_.z, emitterData_.velocityRandomRangeEnd_.z)
        );
    }
    else
    {
        datum.velocity_ = emitterData_.velocityFixed_;
    }

    // 初期の色
    datum.beginColor_ = emitterData_.beginColor_;
    // 終了の色
    datum.endColor_ = emitterData_.endColor_;

    // アルファ値の変化量
    datum.alphaDeltaValue_ = emitterData_.alphaDeltaValue_;
    // 消去条件
    datum.deleteCondition_ = ParticleDeleteCondition::LifeTime;
    datum.accGravity_ = emitterData_.gravity_;
    datum.accResistance_ = emitterData_.resistance_;

    aabb_->SetMinMax(emitterData_.beginPosition_, emitterData_.endPosition_);
}
