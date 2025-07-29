#include "./LevelHelper.h"

#include <Utility/JSONIO/jsonio.h>

SceneObjects Helper::Level::LoadScene(const std::string& _path, ModelManager* _pModelManager)
{
    const auto& json = JSONIO::GetInstance()->Load(_path);
    BlenderLevel::LevelData levelData;
    levelData = json;

    SceneObjects sceneObjects;
    sceneObjects.Initialize();
    sceneObjects.SetLevelData(levelData);
    sceneObjects.Build(_pModelManager);
    return sceneObjects;
}
