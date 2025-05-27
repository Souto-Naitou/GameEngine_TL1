#include "EmitterManager.h"
#include <type_traits>

const EmitterData& EmitterManager::LoadFile(const std::string& _path)
{
    if (!pjsonio_) pjsonio_ = JSONIO::GetInstance();

    EmitterData data = {};
    auto& root = pjsonio_->Load(_path);

    Deserialize(root, data);

    emitterMap_[_path] = data;

    return emitterMap_[_path];
}

const EmitterData& EmitterManager::ReloadFile(const std::string& _path)
{
    if (!pjsonio_) pjsonio_ = JSONIO::GetInstance();
    pjsonio_->Unload(_path);
    return LoadFile(_path);
}

void EmitterManager::SaveFile(const std::string& _path, const EmitterData& _data)
{
    pjsonio_->Save(_path, _data);
}

void EmitterManager::Deserialize(const json& _root, EmitterData& _data)
{
    _data = _root;
}

void EmitterManager::Serialize(json& _root, const EmitterData& _data)
{
    _root = _data;
}
