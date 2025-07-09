#pragma once

#include <Interfaces/ISceneFactory.h>
#include <Scene/SceneBase.h>

#include <memory>

class SceneFactory : public ISceneFactory
{
public:
    SceneFactory() = default;

    /// <summary>
    /// シーン生成
    /// </summary>
    /// <param name="_sceneName">シーン名</param>
    /// <returns>生成したシーン</returns>
    std::unique_ptr<SceneBase> CreateScene(const std::string& _sceneName, ISceneArgs* _pArgs) override;
};