#pragma once

#include <Interfaces/IScene.h>
#include <Features/Object3d/Object3d.h>
#include <Features/GameEye/GameEye.h>
#include <Common/structs.h>

class PG3PT1 : public IScene
{
public:
    PG3PT1() = default;
    ~PG3PT1() = default;
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw2dBackGround() override;
    void Draw3d() override;
    void Draw2dMidground() override;
    void Draw3dMidground() override;
    void DrawLine() override;
    void Draw2dForeground() override;
    void DrawTexts() override;

private:
    std::unique_ptr<Object3d> pBunny_ = nullptr;
    std::unique_ptr<GameEye> pGameEye_ = nullptr;

    DirectionalLight directionalLight_ = {};
};