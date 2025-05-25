#include "EmitterData.h"

#include <Utility/JSONConvTypeFuncs/JSONConvTypeFuncs.h>
#include <iostream>

void from_json(const nlohmann::json& _j, EmitterData& _data)
{
    _j.at("name")                       .get_to(_data.name_);
    _j.at("startScale")                 .get_to(_data.startScale_);
    _j.at("endScale")                   .get_to(_data.endScale_);
    _j.at("emitInterval")               .get_to(_data.emitInterval_);
    _j.at("emitNum")                    .get_to(_data.emitNum_);
    _j.at("emitterLifeTime")            .get_to(_data.emitterLifeTime_);
    _j.at("particleLifeTime")           .get_to(_data.particleLifeTime_);
    _j.at("scaleDelayTime")             .get_to(_data.scaleDelayTime_);
    _j.at("beginPosition")              .get_to(_data.beginPosition_);
    _j.at("endPosition")                .get_to(_data.endPosition_);
    _j.at("emitPositionFixed")          .get_to(_data.emitPositionFixed_);
    _j.at("beginColor")                 .get_to(_data.beginColor_);
    _j.at("endColor")                   .get_to(_data.endColor_);
    _j.at("alphaDeltaValue")            .get_to(_data.alphaDeltaValue_);
    _j.at("velocityRandomRangeBegin")   .get_to(_data.velocityRandomRangeBegin_);
    _j.at("velocityRandomRangeEnd")     .get_to(_data.velocityRandomRangeEnd_);
    _j.at("velocityFixed")              .get_to(_data.velocityFixed_);
    _j.at("gravity")                    .get_to(_data.gravity_);
    _j.at("resistance")                 .get_to(_data.resistance_);
    _j.at("enableRandomVelocity")       .get_to(_data.enableRandomVelocity_);
    _j.at("enableRandomEmit")           .get_to(_data.enableRandomEmit_);
}

void to_json(nlohmann::json& _j, const EmitterData& _data)
{
    _j["name"]                          = _data.name_;
    _j["startScale"]                    = _data.startScale_;
    _j["endScale"]                      = _data.endScale_;
    _j["emitInterval"]                  = _data.emitInterval_;
    _j["emitNum"]                       = _data.emitNum_;
    _j["emitterLifeTime"]               = _data.emitterLifeTime_;
    _j["particleLifeTime"]              = _data.particleLifeTime_;
    _j["scaleDelayTime"]                = _data.scaleDelayTime_;
    _j["beginPosition"]                 = _data.beginPosition_;
    _j["endPosition"]                   = _data.endPosition_;
    _j["emitPositionFixed"]             = _data.emitPositionFixed_;
    _j["beginColor"]                    = _data.beginColor_;
    _j["endColor"]                      = _data.endColor_;
    _j["alphaDeltaValue"]               = _data.alphaDeltaValue_;
    _j["velocityRandomRangeBegin"]      = _data.velocityRandomRangeBegin_;
    _j["velocityRandomRangeEnd"]        = _data.velocityRandomRangeEnd_;
    _j["velocityFixed"]                 = _data.velocityFixed_;
    _j["gravity"]                       = _data.gravity_;
    _j["resistance"]                    = _data.resistance_;
    _j["enableRandomVelocity"]          = _data.enableRandomVelocity_;
    _j["enableRandomEmit"]              = _data.enableRandomEmit_;
}
