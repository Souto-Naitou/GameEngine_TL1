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

    void ParseJsonValue(JsonValue& _loader, EmitterData& _data);
    JsonValue ParseEmitterData(const EmitterData& _data);

    Vector3 ParseVector3(JsonObject& _obj);
    Vector4 ParseVector4(JsonObject& _obj);
    Color ParseColor(JsonObject& _obj);

    std::shared_ptr<JsonValue> ParseVector3(const Vector3& _vec);
    std::shared_ptr<JsonValue> ParseVector4(const Vector4& _vec);
    std::shared_ptr<JsonValue> ParseColor(const Color& _color);

private:
    std::unordered_map<std::filesystem::path, EmitterData> emitterMap_;
};