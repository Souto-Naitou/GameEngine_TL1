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


private:
    EmitterManager() = default;
    ~EmitterManager() = default;

    void ParseEmitterData(JsonValue& _loader, EmitterData& _data);

    Vector3 ParseVector3(JsonObject& _obj);
    Vector4 ParseVector4(JsonObject& _obj);
    Color ParseColor(JsonObject& _obj);

private:
    std::unordered_map<std::filesystem::path, EmitterData> emitterMap_;

};