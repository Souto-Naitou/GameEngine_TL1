#include "SendModelList.h"
#include "ReloadScene.h"
#include <Features/LevelLoader/LevelHelper.h>

void OEReloadScene::Run()
{
    Helper::Level::Unload(path_);
    *pSceneObjects_ = Helper::Level::LoadScene(path_, pModelManager_);
}

std::string OEReloadScene::Serialize() const
{
    return{};
}

void OEReloadScene::SetPath(const std::string& _path)
{
    path_ = _path;
}
