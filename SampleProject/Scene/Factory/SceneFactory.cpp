#include "SceneFactory.h"

#include <Scene/GameScene/GameScene.h>
#include <Scene/RequiredScene/RequiredScene.h>

#include <cassert>

IScene* SceneFactory::CreateScene(const std::string& _sceneName)
{
    IScene* pNewScene = nullptr;

    if (_sceneName == "RequiredScene")
    {
        pNewScene = new RequiredScene();
    }
    else if (_sceneName == "GameScene")
    {
        pNewScene = new GameScene();
    }

    assert(pNewScene);

    return pNewScene;
}
