#include "EmitterManager.h"

const EmitterData& EmitterManager::LoadFile(std::string _path)
{
    JsonLoader& loader = JsonLoader::GetInstance();

    EmitterData data = {};
    loader.LoadFile(_path);
    JsonValue root = loader[_path];

    ParseEmitterData(root, data);

    emitterMap_[_path] = data;

    return emitterMap_[_path];
}

const EmitterData& EmitterManager::ReloadFile(std::string _path)
{
    JsonLoader& loader = JsonLoader::GetInstance();

    EmitterData data = {};
    loader.ReloadFile(_path);

    JsonValue root = loader[_path];

    ParseEmitterData(root, data);

    emitterMap_[_path] = data;

    return emitterMap_[_path];
}

void EmitterManager::ParseEmitterData(JsonValue& _loader, EmitterData& _data)
{
    JsonObject obj = {};

    /// [Transform]

    obj = _loader["startScale"];
    _data.startScale_ = ParseVector3(obj);

    obj = _loader["endScale"];
    _data.endScale_ = ParseVector3(obj);

    _data.scaleDelayTime_ = _loader["scaleDelayTime"];


    /// [Common]

    _data.particleLifeTime_ = _loader["particleLifeTime"];

    _data.emitInterval_ = _loader["emitInterval"];

    _data.emitNum_ = _loader["emitNum"];

    _data.emitterLifeTime_ = _loader["emitterLifeTime"];


    /// [Generate]

    _data.enableRandomEmit_ = _loader["enableRandomEmit"];

    obj = _loader["beginPosition"];
    _data.beginPosition_ = ParseVector3(obj);

    obj = _loader["endPosition"];
    _data.endPosition_ = ParseVector3(obj);

    obj = _loader["emitPositionFixed"];
    _data.emitPositionFixed_ = ParseVector3(obj);


    /// [Velocity]

    _data.enableRandomVelocity_ = _loader["enableRandomVelocity"];

    obj = _loader["velocityRandomRangeBegin"];
    _data.velocityRandomRangeBegin_ = ParseVector3(obj);

    obj = _loader["velocityRandomRangeEnd"];
    _data.velocityRandomRangeEnd_ = ParseVector3(obj);

    obj = _loader["velocityFixed"];
    _data.velocityFixed_ = ParseVector3(obj);


    /// [Color]

    obj = _loader["beginColor"];
    _data.beginColor_ = ParseColor(obj);

    obj = _loader["endColor"];
    _data.endColor_ = ParseColor(obj);

    _data.alphaDeltaValue_ = _loader["alphaDeltaValue"];


    /// [Physics]

    obj = _loader["gravity"];
    _data.gravity_ = ParseVector3(obj);

    obj = _loader["resistance"];
    _data.resistance_ = ParseVector3(obj);


    return;
}

Vector3 EmitterManager::ParseVector3(JsonObject& _obj)
{
    float x, y, z;
    x = *_obj["x"]; y = *_obj["y"]; z = *_obj["z"];
    return Vector3(x,y,z);
}

Vector4 EmitterManager::ParseVector4(JsonObject& _obj)
{
    float x, y, z, w;
    x = *_obj["x"]; y = *_obj["y"]; z = *_obj["z"]; w = *_obj["w"];
    return Vector4(x, y, z, w);
}

Color EmitterManager::ParseColor(JsonObject& _obj)
{
    float r, g, b, a;
    r = *_obj["r"]; g = *_obj["g"]; b = *_obj["b"]; a = *_obj["a"];
    return Color(r, g, b, a);
}
