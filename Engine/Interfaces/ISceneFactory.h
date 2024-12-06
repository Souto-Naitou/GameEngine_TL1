#pragma once

#include "IScene.h"
#include <string>

/// <summary>
/// シーンファクトリ インターフェース
/// </summary>
class ISceneFactory
{
public:
    virtual ~ISceneFactory() = default;

    virtual IScene* CreateScene(const std::string& _sceneName) = 0;
};