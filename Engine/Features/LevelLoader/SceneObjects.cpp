#include "./SceneObjects.h"
#include <Features/Model/ObjModel.h>
#include <utility>

void SceneObjects::Initialize()
{

}

void SceneObjects::Finalize()
{
    objects_.clear();
}

void SceneObjects::Update()
{
    for (auto& model : models_)
    {
        if (model)
        {
            model->Update();
        }
    }

    for (auto& obj : objects_)
    {
        if (obj)
        {
            obj->Update();
        }
    }
}

void SceneObjects::Draw()
{
    for (const auto& obj : objects_)
    {
        if (obj)
        {
            obj->Draw();
        }
    }
}

std::string SceneObjects::GetName() const
{
    return levelData_.name.empty() ? "Unnamed Scene" : levelData_.name;
}

void SceneObjects::SetLevelData(const BlenderLevel::LevelData& _levelData)
{
    levelData_ = _levelData;
}

void SceneObjects::Build(ModelManager* _modelManager)
{
    for (const auto& object : levelData_.objects)
    {
        if (object.type == "MESH")
        {
            // Create a model from the object data
            auto model = _modelManager->Load(object.filename);
            std::shared_ptr<ObjModel> objModel = std::make_shared<ObjModel>();
            objModel->Clone(model);
            models_.emplace_back(std::move(objModel));

            // Create an Object3d instance
            std::shared_ptr<Object3d> object3d = std::make_shared<Object3d>();
            object3d->Initialize();
            object3d->SetModel(models_.back().get());
            object3d->SetName(object.name);
            object3d->SetScale(object.transform.scale);
            object3d->SetRotate(object.transform.rotate);
            object3d->SetTranslate(object.transform.translate);
            objects_.emplace_back(std::move(object3d));
        }
    }
}