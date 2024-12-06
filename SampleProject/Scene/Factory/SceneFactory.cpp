#include "SceneFactory.h"

#include <Scene/GameScene.h>
#include <cassert>

IScene* SceneFactory::CreateScene(const std::string& _sceneName)
{
    IScene* pNewScene = nullptr;

    if (_sceneName == "GameScene")
    {
        pNewScene = new GameScene();
    }

    assert(pNewScene);

    return pNewScene;
}
