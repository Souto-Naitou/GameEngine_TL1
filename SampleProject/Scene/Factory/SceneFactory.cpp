#include "SceneFactory.h"

#include <Scene/GameScene/GameScene.h>
#include <Scene/InstancingScene/InstancingScene.h>
#include <Scene/CG3PT2/CG3PT2.h>

#include <cassert>

#define JUDGE_SCENE(class) if (_sceneName == #class) { return std::make_unique<class>(); }

std::unique_ptr<IScene> SceneFactory::CreateScene(const std::string & _sceneName)
{
    JUDGE_SCENE(InstancingScene)
    else JUDGE_SCENE(GameScene)
    else JUDGE_SCENE(CG3PT2)

    assert(false && "シーンの生成に失敗しました");

    return nullptr;
}
