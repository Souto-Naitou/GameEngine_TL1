#pragma once

#include "IScene.h"
#include <string>

#include <memory>

/// <summary>
/// シーンファクトリ インターフェース
/// </summary>
class ISceneFactory
{
public:
    virtual ~ISceneFactory() = default;

    virtual std::unique_ptr<IScene> CreateScene(const std::string& _sceneName) = 0;
};