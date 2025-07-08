#include "./ModelLoader.h"

#include <Core/ConfigManager/ConfigManager.h>
#include <Utility/FileSystem/fsutl.h>
#include <Features/Model/ModelFromObj.h>
#include <Features/Model/Helper/ModelHelper.h>


void ModelLoader::Initialize()
{
    pThreadPool_ = ThreadPool::GetInstance();
}

void ModelLoader::Update()
{
    if (!uploadQueue_.empty())
    {
        auto frontModel = uploadQueue_.front();
        frontModel->CreateGPUResource();
        uploadQueue_.pop();
    }
}

std::string ModelLoader::GetDirectoryPath(const std::string& _fileName)
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
    else
    {
        auto path = std::filesystem::path(_fileName);
        directoryPath = path.parent_path().string();
    }

    return directoryPath;
}

std::shared_ptr<IModel> ModelLoader::LoadModel(const std::string& _path)
{
    std::filesystem::path fsPath = _path;
    
    // OBJファイルでなければ例外を投げる
    if (fsPath.extension() != ".obj")
    {
        throw std::runtime_error("File is not the obj format");
        return nullptr;
    }

    std::shared_ptr<ModelFromObj> model = std::make_unique<ModelFromObj>();
    model->SetDirectX12(pDx12_);
    model->Initialize();

    auto loadObj = [model, this, _path]() -> void
    {
        *model->GetModelData() = Helper::Model::LoadObjFile({}, _path);
        this->EnqueueUpload(model.get());
    };

    pThreadPool_->enqueue(loadObj);

    return model;
}

void ModelLoader::LoadModelAll()
{
    for (const auto& path : autoLoadPaths_)
    {
        for (auto itr = std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::skip_permission_denied);
            itr != std::filesystem::recursive_directory_iterator();
            ++itr)
        {
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

void ModelLoader::EnqueueUpload(IModel* _ptr)
{
    std::lock_guard<std::mutex> lock(mtx_);
    uploadQueue_.push(_ptr);
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

void ModelLoader::AddAutoLoadPath(const std::string& _path)
{
    for (const auto& path : searchPaths_)
    {
        if (path == _path)
        {
            return;
        }
    }

    autoLoadPaths_.push_back(_path);
}
