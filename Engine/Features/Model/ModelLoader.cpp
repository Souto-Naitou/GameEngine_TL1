#include "./ModelLoader.h"

#include <Core/ConfigManager/ConfigManager.h>

void ModelLoader::Initialize()
{
    // Configに記述されているフォルダの追加
    auto& cfgData = ConfigManager::GetInstance()->GetConfigData();
    for (auto& path : cfgData.model_paths)
    {
        this->AddAutoLoadPath(path);
    }
}

void ModelLoader::AddAutoLoadPath(const std::string& _path)
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

void ModelLoader::LoadAllModel()
{
    for (const auto& path : autoLoadPaths_)
    {
        for (auto itr = std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::skip_permission_denied);
            itr != std::filesystem::recursive_directory_iterator();
            ++itr){
            std::filesystem::path objPath = itr->path();
            if (objPath.extension() != ".obj")
            {
                continue;
            }
            AddAutoLoadPath(objPath.parent_path().string());
            LoadModel(objPath.string());
        }
    }
}

std::string ModelLoader::GetDirectoryPath(std::string _fileName)
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

void ModelLoader::LoadModel(const std::string& _filePath, const std::string& _texturePath)
{
    std::filesystem::path fullpath = GetDirectoryPath(_filePath);
    if (fullpath.empty()) fullpath = _filePath;
    else fullpath += "/" + _filePath;

    for ( auto& model : models_ )
    {
        std::filesystem::path fsModelPath = GetLowerPath(model.first.string());
        std::filesystem::path fsFullPath = GetLowerPath(fullpath.string());

        if ( fsModelPath == fsFullPath )
        {
            return;
        }
    }

    std::unique_ptr<Model> model = std::make_unique<Model>();
    model->SetDirectX12(pDx12_);
    model->Initialize(fullpath.string(), _texturePath);
    models_.emplace(fullpath, std::move(model));
}


void ModelLoader::AddSearchPath(const std::string& _path)
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
