#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <list>
#include <queue>
#include "Model.h"
#include <filesystem>
#include <mutex>
#include <Core/DirectX12/DirectX12.h>

class Particle;

class ModelManager : public EngineFeature
{
public:
    ModelManager(ModelManager&) = delete;
    ModelManager(ModelManager&&) = delete;
    ModelManager& operator=(ModelManager&) = delete;
    ModelManager& operator=(ModelManager&&) = delete;

    static ModelManager* GetInstance()
    {
        static ModelManager instance;
        return &instance;
    }

    void Initialize();
    void Update();


private:
    std::queue<Model*> uploadQueue_;
    std::queue<Particle*> uploadQueueParticle_;

    ModelManager() = default;
    ~ModelManager() = default;

    std::filesystem::path GetLowerPath(const std::string& _path);
};