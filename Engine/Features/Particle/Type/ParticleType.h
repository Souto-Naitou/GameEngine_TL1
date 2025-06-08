#pragma once
#include <Timer/Timer.h>
#include <Common/structs.h>
#include <Range.h>
#include <Vector3.h>
#include <Vector4.h>
#include "../Emitter/EmitterData.h"

using namespace  Type::ParticleEmitter;

enum class ParticleDeleteCondition
{
    LifeTime,
    ZeroAlpha,
};

struct ParticleData
{
    Timer                                   timer_              = {};
    Transform                               transform_          = {};
    Range<Vector3>                          scaleRange_         = {};
    Vector3                                 acceleration_       = {};
    Vector4                                 currentColor_       = {};
    Range<Vector4>                          colorRange_         = {};
    float                                   scaleDelayTime_     = 0.0f;
    float                                   alphaDeltaValue_    = 0.0f;
    float                                   lifeTime_           = 0.0f;
    float                                   currentLifeTime_    = 0.0f;
    Vector3                                 accResistance_      = {};
    Vector3                                 accGravity_         = {};
    Vector3                                 velocity_           = {};
    float                                   frictionCoef_       = {};
    bool                                    enableDirectionByVelocity = false;
    bool                                    enableCollisionFloor = false;
    float                                   radius              = 0.0f;
    v3::CollisionFloor                      collisionFloor_     = {};
    ParticleDeleteCondition                 deleteCondition_    = ParticleDeleteCondition::LifeTime;
};