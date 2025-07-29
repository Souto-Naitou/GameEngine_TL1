#pragma once

#include <Features/Object3d/Object3d.h>
#include <list>
#include <memory>
#include <d3d12.h>
#include <string>
#include <Features/LevelLoader/ISceneObjects.h>
#include <Features/LevelLoader/LevelData.h>
#include <Features/Model/ModelManager.h>
#include <Features/Model/IModel.h>

class SceneObjects : public ISceneObjects
{
public:
    SceneObjects() = default;
    ~SceneObjects() = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;

    std::string GetName() const override;

    void SetLevelData(const BlenderLevel::LevelData& _levelData);
    void Build(ModelManager* _modelManager);

private:
    std::list<std::shared_ptr<Object3d>> objects_;
    std::list<std::shared_ptr<IModel>> models_;
    BlenderLevel::LevelData levelData_; // レベルデータ
};