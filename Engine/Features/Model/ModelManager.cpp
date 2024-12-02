#include "ModelManager.h"
#include <Features/Model/Helper/ModelHelper.h>
#include <filesystem>
#include <Features/Particle/Particle.h>

void ModelManager::Initialize()
{
}

void ModelManager::AddSearchPath(const std::string& _path)
{
    for (const auto& path : searchPaths_)
    {
        if (path == _path)
        {
            return;
        }
    }

    searchPaths_.push_back(_path);
}

void ModelManager::AddAutoLoadPath(const std::string& _path)
{
    for (const auto& path : searchPaths_)
    {
        if (path == _path)
        {
            return;
        }
    }

    AddSearchPath(_path);

    autoLoadPaths_.push_back(_path);
}

void ModelManager::LoadAllModel()
{
    for (const auto& path : autoLoadPaths_)
    {
        for (auto itr = std::filesystem::directory_iterator(path, std::filesystem::directory_options::skip_permission_denied);
            itr != std::filesystem::directory_iterator();
            ++itr){
            std::filesystem::path objPath = itr->path();
            if (objPath.extension() != ".obj")
            {
                continue;
            }
            LoadModel(objPath.string());
        }
    }
}

std::string ModelManager::GetDirectoryPath(std::string _fileName)
{
    std::string directoryPath = {};

    if (!std::filesystem::exists(_fileName))
    {
        std::filesystem::path argPath = _fileName;
        for (const auto& strPath : searchPaths_)
        {
            std::filesystem::path path = strPath;
            std::filesystem::path searchPath = path / argPath;
            if (std::filesystem::exists(searchPath))
            {
                directoryPath = strPath;
                break;
            }
        }
    }

    return directoryPath;
}

void ModelManager::LoadModel(const std::string& _filePath)
{
    std::string fullpath = GetDirectoryPath(_filePath);
    if (fullpath.empty()) fullpath = _filePath;
    else fullpath += "/" + _filePath;

    if (models_.contains(fullpath))
    {
        return;
    }
    std::unique_ptr<Model> model = std::make_unique<Model>();

    model->Initialize(fullpath);
    models_.emplace(fullpath, std::move(model));
}

Model* ModelManager::FindModel(const std::string& _filePath)
{
    std::string fullpath = GetDirectoryPath(_filePath);
    if (fullpath.empty()) fullpath = _filePath;
    else fullpath += "/" + _filePath;

    return models_[fullpath].get();
}

void ModelManager::Update()
{
    if (!uploadQueue_.empty())
    {
        uploadQueue_.front()->Upload();
        uploadQueue_.pop();
    }
    if (!uploadQueueParticle_.empty())
    {
        uploadQueueParticle_.front()->Upload();
        uploadQueueParticle_.pop();
    }
}
