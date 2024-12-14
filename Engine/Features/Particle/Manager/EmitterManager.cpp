#include "EmitterManager.h"

const EmitterData& EmitterManager::LoadFile(std::string _path)
{
    JsonLoader& loader = JsonLoader::GetInstance();

    EmitterData data = {};
    loader.LoadFile(_path);
    JsonValue root = loader[_path];

    ParseJsonValue(root, data);

    emitterMap_[_path] = data;

    return emitterMap_[_path];
}

const EmitterData& EmitterManager::ReloadFile(std::string _path)
{
    JsonLoader& loader = JsonLoader::GetInstance();

    if (!loader.IsExist(_path)) return LoadFile(_path);

    loader.ReloadFile(_path);

    JsonValue root = loader[_path];

    EmitterData data = {};
    ParseJsonValue(root, data);

    emitterMap_[_path] = data;

    return emitterMap_[_path];
}

void EmitterManager::SaveFile(const std::string& _path, const EmitterData& _data)
{
    JsonLoader& loader = JsonLoader::GetInstance();
    JsonValue data = ParseEmitterData(_data);
    loader.SaveFile(_path, data);
}

void EmitterManager::ParseJsonValue(JsonValue& _loader, EmitterData& _data)
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

JsonValue EmitterManager::ParseEmitterData(const EmitterData& _data)
{
    JsonValue value = {};
    JsonObject root = {};

    /// [Transform]

    root["startScale"] = ParseVector3(_data.startScale_);
    root["endScale"] = ParseVector3(_data.endScale_);
    root["scaleDelayTime"] = std::make_shared<JsonValue>(JsonValueTypes::Float, _data.scaleDelayTime_);


    /// [Common]

    root["particleLifeTime"] = std::make_shared<JsonValue>(JsonValueTypes::Float, _data.particleLifeTime_);
    root["emitInterval"] = std::make_shared<JsonValue>(JsonValueTypes::Float, _data.emitInterval_);
    root["emitNum"] = std::make_shared<JsonValue>(JsonValueTypes::Int, _data.emitNum_);
    root["emitterLifeTime"] = std::make_shared<JsonValue>(JsonValueTypes::Float, _data.emitterLifeTime_);


    /// [Generate]


    root["enableRandomEmit"] = std::make_shared<JsonValue>(JsonValueTypes::Bool, _data.enableRandomEmit_);
    root["beginPosition"] = ParseVector3(_data.beginPosition_);
    root["endPosition"] = ParseVector3(_data.endPosition_);
    root["emitPositionFixed"] = ParseVector3(_data.emitPositionFixed_);


    /// [Velocity]

    root["enableRandomVelocity"] = std::make_shared<JsonValue>(JsonValueTypes::Bool, _data.enableRandomVelocity_);
    root["velocityRandomRangeBegin"] = ParseVector3(_data.velocityRandomRangeBegin_);
    root["velocityRandomRangeEnd"] = ParseVector3(_data.velocityRandomRangeEnd_);
    root["velocityFixed"] = ParseVector3(_data.velocityFixed_);


    /// [Color]

    root["beginColor"] = ParseColor(_data.beginColor_);
    root["endColor"] = ParseColor(_data.endColor_);
    root["alphaDeltaValue"] = std::make_shared<JsonValue>(JsonValueTypes::Float, _data.alphaDeltaValue_);


    /// [Physics]

    root["gravity"] = ParseVector3(_data.gravity_);
    root["resistance"] = ParseVector3(_data.resistance_);


    value.type = JsonValueTypes::Object;
    value.value = root;

    return value;
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

std::shared_ptr<JsonValue> EmitterManager::ParseVector3(const Vector3& _vec)
{
    auto result = std::make_shared<JsonValue>();
    JsonObject obj = {};
    auto x = std::make_shared<JsonValue>();
    auto y = std::make_shared<JsonValue>();
    auto z = std::make_shared<JsonValue>();
    x->type = JsonValueTypes::Float;
    y->type = JsonValueTypes::Float;
    z->type = JsonValueTypes::Float;
    x->value = _vec.x;
    y->value = _vec.y;
    z->value = _vec.z;
    obj["x"] = x;
    obj["y"] = y;
    obj["z"] = z;

    result->type = JsonValueTypes::Object;
    result->value = obj;

    return result;
}

std::shared_ptr<JsonValue> EmitterManager::ParseVector4(const Vector4& _vec)
{
    auto result = std::make_shared<JsonValue>();
    JsonObject obj = {};
    auto x = std::make_shared<JsonValue>();
    auto y = std::make_shared<JsonValue>();
    auto z = std::make_shared<JsonValue>();
    auto w = std::make_shared<JsonValue>();

    x->type = JsonValueTypes::Float;
    y->type = JsonValueTypes::Float;
    z->type = JsonValueTypes::Float;
    w->type = JsonValueTypes::Float;

    x->value = _vec.x;
    y->value = _vec.y;
    z->value = _vec.z;
    w->value = _vec.w;

    obj["x"] = x;
    obj["y"] = y;
    obj["z"] = z;
    obj["w"] = w;

    result->type = JsonValueTypes::Object;
    result->value = obj;

    return result;
}

std::shared_ptr<JsonValue> EmitterManager::ParseColor(const Color& _color)
{
    auto result = std::make_shared<JsonValue>();
    JsonObject obj = {};
    auto r = std::make_shared<JsonValue>();
    auto g = std::make_shared<JsonValue>();
    auto b = std::make_shared<JsonValue>();
    auto a = std::make_shared<JsonValue>();

    r->type = JsonValueTypes::Float;
    g->type = JsonValueTypes::Float;
    b->type = JsonValueTypes::Float;
    a->type = JsonValueTypes::Float;

    r->value = static_cast<float>(_color.rgba.r) / 255.0f;
    g->value = static_cast<float>(_color.rgba.g) / 255.0f;
    b->value = static_cast<float>(_color.rgba.b) / 255.0f;
    a->value = static_cast<float>(_color.rgba.a) / 255.0f;
    obj["r"] = r;
    obj["g"] = g;
    obj["b"] = b;
    obj["a"] = a;

    result->type = JsonValueTypes::Object;
    result->value = obj;

    return result;
}
