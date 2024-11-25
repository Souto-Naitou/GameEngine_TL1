#pragma once

#include <DirectX12.h>
#include <unordered_map>
#include <string>
#include <Model.h>
#include <memory>

class ModelManager
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
    void LoadModel(const std::string& _filePath);
    Model* FindModel(const std::string& _filePath) const { return models_.at(_filePath).get(); }

private:
    std::unordered_map<std::string, std::unique_ptr<Model>> models_;

    ModelManager() = default;
    ~ModelManager() = default;
};