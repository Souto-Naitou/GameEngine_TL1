#pragma once

#include <Scene/SceneBase.h>
#include <Features/Object3d/Object3d.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Input/Input.h>
#include <Common/structs.h>
#include <Features/Model/ModelManager.h>

class PG3PT1 : public SceneBase
{
public:
    PG3PT1(ISceneArgs* pArgs_) : SceneBase(pArgs_) {}
    ~PG3PT1() = default;
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawTexts() override;

private:
    IModel*                     pModelBunny_      = nullptr;
    std::unique_ptr<Object3d>   pBunny_           = nullptr;
    std::unique_ptr<GameEye>    pGameEye_         = nullptr;
    DirectionalLight            directionalLight_ = {};
    bool                        isOpen_           = true;

    // Pointers
    Input* pInput_ = nullptr;
    ModelManager* pModelManager_ = nullptr;
};