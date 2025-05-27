#include "EmitterData.h"

#include <Utility/JSON/jsonutl.h>
#include <Utility/JSONConvTypeFuncs/JSONConvTypeFuncs.h>
#include <iostream>

void from_json(const nlohmann::json& _j, EmitterData& _data)
{
    utl::json::try_assign(_j, "name", _data.name_);
    utl::json::try_assign(_j, "scaleFixed", _data.scaleFixed_);
    utl::json::try_assign(_j, "scaleRange", _data.scaleRange_);
    utl::json::try_assign(_j, "scaleRandomRange", _data.scaleRandomRange_);
    utl::json::try_assign(_j, "emitInterval", _data.emitInterval_);
    utl::json::try_assign(_j, "emitNum", _data.emitNum_);
    utl::json::try_assign(_j, "emitterLifeTime", _data.emitterLifeTime_);
    utl::json::try_assign(_j, "particleLifeTime", _data.particleLifeTime_);
    utl::json::try_assign(_j, "scaleDelayTime", _data.scaleDelayTime_);
    utl::json::try_assign(_j, "positionRange", _data.positionRange_);
    utl::json::try_assign(_j, "emitPositionFixed", _data.emitPositionFixed_);
    utl::json::try_assign(_j, "colorRange", _data.colorRange_);
    utl::json::try_assign(_j, "alphaDeltaValue", _data.alphaDeltaValue_);
    utl::json::try_assign(_j, "velocityRandomRange", _data.velocityRandomRange_);
    utl::json::try_assign(_j, "velocityFixed", _data.velocityFixed_);
    utl::json::try_assign(_j, "rotationRandomRange", _data.rotationRandomRange_);
    utl::json::try_assign(_j, "gravity", _data.gravity_);
    utl::json::try_assign(_j, "resistance", _data.resistance_);
    utl::json::try_assign(_j, "enableRandomVelocity", _data.enableRandomVelocity_);
    utl::json::try_assign(_j, "enableRandomEmit", _data.enableRandomEmit_);
    utl::json::try_assign(_j, "enableRandomRotation", _data.enableRandomRotation_);
    utl::json::try_assign(_j, "enableRandomScale", _data.enableRandomScale_);
    utl::json::try_assign(_j, "enableScaleTransition", _data.enableScaleTransition_);
}

void to_json(nlohmann::json& _j, const EmitterData& _data)
{
    _j["name"]                          = _data.name_;
    _j["scaleFixed"]                    = _data.scaleFixed_;
    _j["scaleRange"]                    = _data.scaleRange_;
    _j["scaleRandomRange"]              = _data.scaleRandomRange_;
    _j["emitInterval"]                  = _data.emitInterval_;
    _j["emitNum"]                       = _data.emitNum_;
    _j["emitterLifeTime"]               = _data.emitterLifeTime_;
    _j["particleLifeTime"]              = _data.particleLifeTime_;
    _j["scaleDelayTime"]                = _data.scaleDelayTime_;
    _j["positionRange"]                 = _data.positionRange_;
    _j["emitPositionFixed"]             = _data.emitPositionFixed_;
    _j["colorRange"]                    = _data.colorRange_;
    _j["alphaDeltaValue"]               = _data.alphaDeltaValue_;
    _j["velocityRandomRange"]           = _data.velocityRandomRange_;
    _j["velocityFixed"]                 = _data.velocityFixed_;
    _j["rotationRandomRange"]           = _data.rotationRandomRange_;
    _j["gravity"]                       = _data.gravity_;
    _j["resistance"]                    = _data.resistance_;
    _j["enableRandomVelocity"]          = _data.enableRandomVelocity_;
    _j["enableRandomEmit"]              = _data.enableRandomEmit_;
    _j["enableRandomRotation"]          = _data.enableRandomRotation_;
    _j["enableRandomScale"]             = _data.enableRandomScale_;
    _j["enableScaleTransition"]         = _data.enableScaleTransition_;
}
