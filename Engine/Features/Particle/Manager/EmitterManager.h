#pragma once

#include <Features/Particle/Emitter/ParticleEmitter.h>
#include <unordered_map>
#include <filesystem>
#include <JsonParser/JsonLoader.h>

class EmitterManager
{
public:
    EmitterManager(const EmitterManager&) = delete;
    EmitterManager& operator=(const EmitterManager&) = delete;
    EmitterManager(EmitterManager&&) = delete;
    EmitterManager& operator=(EmitterManager&&) = delete;

    static EmitterManager* GetInstance()
    {
        static EmitterManager instance;
        return &instance;
    }

    const EmitterData& LoadFile(std::string _path);
    const EmitterData& ReloadFile(std::string _path);
    void SaveFile(const std::string& _path, const EmitterData& _data);


private:
    EmitterManager() = default;
    ~EmitterManager() = default;

    void ParseJsonValue(Json::Value& _loader, EmitterData& _data);
    Json::Value ParseEmitterData(const EmitterData& _data);

    Vector3 ParseVector3(Json::Object& _obj);
    Vector4 ParseVector4(Json::Object& _obj);
    Color ParseColor(Json::Object& _obj);

    std::shared_ptr<Json::Value> ParseVector3(const Vector3& _vec);
    std::shared_ptr<Json::Value> ParseVector4(const Vector4& _vec);
    std::shared_ptr<Json::Value> ParseColor(const Color& _color);

private:
    std::unordered_map<std::filesystem::path, EmitterData> emitterMap_;
};