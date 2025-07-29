#pragma once
#include <string>

#include <Utility/JSONIO/jsonio.h>
#include <Features/LevelLoader/SceneObjects.h>

namespace Helper::Level
{
    SceneObjects LoadScene(const std::string& _path, ModelManager* _pModelManager);
}