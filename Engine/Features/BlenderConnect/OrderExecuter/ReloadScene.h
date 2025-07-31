#pragma once

#include "IOrderExecuter.h"
#include <Features/LevelLoader/SceneObjects.h>
#include <Features/Model/ModelManager.h>

class OEReloadScene : public IOrderExecuter
{
public:
    OEReloadScene() = default;
    ~OEReloadScene() override = default;
    void Run() override;
    std::string Serialize() const override;

    bool IsNeedResponse() const override { return false; }

    void SetPath(const std::string& _path);
    void SetSceneObjects(SceneObjects* _sceneObjects) { pSceneObjects_ = _sceneObjects; }
    void SetModelManager(ModelManager* _modelManager) { pModelManager_ = _modelManager; }

private:
    std::string path_ = {};
    SceneObjects* pSceneObjects_ = nullptr; // シーンオブジェクトへのポインタ
    ModelManager* pModelManager_ = nullptr; // モデルマネージャーへのポインタ
};