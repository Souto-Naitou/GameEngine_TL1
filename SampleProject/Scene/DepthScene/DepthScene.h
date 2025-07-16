#pragma once

#include <Scene/SceneBase.h>
#include <Features/Object3d/Object3d.h>
#include <Features/Input/Input.h>
#include <Features/GameEye/GameEye.h>

#include <list>
#include <memory>
#include <Features/Model/ModelManager.h>

class DepthScene : public SceneBase
{
public:
    DepthScene(ISceneArgs* pArgs_) : SceneBase(pArgs_) {}
    ~DepthScene() = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawTexts() override;

private:
    IModel* pModelPlane_ = nullptr;
    std::unique_ptr<GameEye> pGameEye_ = nullptr;
    std::list<std::unique_ptr<Object3d>> objectList_;
    float nextObjZ_ = 0;
    const float objZInterval_ = 0.01f;
    unsigned int nextCreateCount_ = 0;
    int nextCreateCountTemp_ = 0;

    // Pointers
    Input* pInput_ = nullptr;
    ModelManager* pModelManager_ = nullptr;

    // Internal functions
    void CreateObject();
    void DebugWindow();
    
    // For debug
    std::string windowName_ = "DepthScene";
};
