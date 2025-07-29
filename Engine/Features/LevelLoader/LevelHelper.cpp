#include "./LevelHelper.h"

#include <Utility/JSONIO/jsonio.h>

SceneObjects Helper::Level::LoadScene(const std::string& _path, ModelManager* _pModelManager)
{
    JSONIO* jsonio = JSONIO::GetInstance();
    const auto& json = jsonio->Load(_path);
    BlenderLevel::LevelData levelData;
    levelData = json;

    SceneObjects sceneObjects;
    sceneObjects.Initialize();
    sceneObjects.SetLevelData(levelData);
    sceneObjects.Build(_pModelManager);
    return sceneObjects;
}

void Helper::Level::Unload(const std::string& _path)
{
    JSONIO* jsonio = JSONIO::GetInstance();
    jsonio->Unload(_path);
}