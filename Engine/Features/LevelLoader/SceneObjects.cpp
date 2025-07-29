#include "./SceneObjects.h"
#include <Features/Model/ObjModel.h>
#include <utility>
#include <Features/GameEye/FreeLook/FreeLookEye.h>
#include <Features/Object3d/Object3dSystem.h>
#include <imgui.h>

void SceneObjects::Initialize()
{

}

void SceneObjects::Finalize()
{
    for (auto& obj : objects_)
    {
        obj->Finalize();
    }
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

    gameeye_->Update();
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

void SceneObjects::ImGui()
{
    ImGui::Button("Reload");
}

void SceneObjects::SetLevelData(const BlenderLevel::LevelData& _levelData)
{
    levelData_ = _levelData;
}

void SceneObjects::SetDirectionalLight(DirectionalLight* _light)
{
    for (auto& obj : objects_)
    {
        if (obj)
        {
            obj->SetEnableLighting(true);
            obj->SetDirectionalLight(_light);
        }
    }
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
            object3d->SetEnableLighting(false);
            objects_.emplace_back(std::move(object3d));
        }
        else if (object.type == "CAMERA")
        {
            gameeye_ = std::make_shared<FreeLookEye>();
            gameeye_->SetTransform(object.transform);
            gameeye_->SetName(object.name);
        }
    }

    // GameEyeを渡す
    Object3dSystem::GetInstance()->SetGlobalEye(gameeye_.get());
}