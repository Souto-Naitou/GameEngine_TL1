#include "ModelManager.h"

#include <Core/ConfigManager/ConfigManager.h>
#include <filesystem>
#include <algorithm>
#include <cctype>

void ModelManager::Initialize()
{

}



Model* ModelManager::FindModel(const std::string& _filePath)
{
    std::string fullpath = GetDirectoryPath(_filePath);
    if (fullpath.empty()) fullpath = _filePath;
    else fullpath += "/" + _filePath;

    auto lowerFullPath = GetLowerPath(fullpath);

    Model* result = nullptr;

    for ( auto& model : models_ )
    {
        auto lowerModelPath = GetLowerPath(model.first.string());
        if ( lowerModelPath == lowerFullPath )
        {
            result = model.second.get();
            break;
        }
    }

    return result;
}

std::filesystem::path ModelManager::GetLowerPath(const std::string& _path)
{
    std::filesystem::path result = _path;
    std::string str = _path;

    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) -> char {
        return static_cast<char>(std::tolower(c));
    });

    result = str;

    return result;
}

void ModelManager::Update()
{
    if (!uploadQueue_.empty())
    {
        uploadQueue_.front()->Upload();
        uploadQueue_.pop();
    }
}
