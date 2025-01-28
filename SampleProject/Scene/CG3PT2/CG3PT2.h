#pragma once

#include <Interfaces/IScene.h>
#include <Features/Object3d/Object3d.h>
#include <Features/GameEye/GameEye.h>

#include <Common/structs.h>

#include <memory>
#include <string>

class CG3PT2 : public IScene
{
public:
    CG3PT2() = default;
    ~CG3PT2() = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw2dBackGround() override;
    void Draw3d() override;
    void Draw2dMidground() override;
    void Draw3dMidground() override;
    void DrawLine() override;
    void Draw2dForeground() override;


private:
    std::unique_ptr<GameEye>    pGameEye_               = nullptr;

    DirectionalLight            directionalLight_       = {};
    PointLight                  pointLight_             = {};

    std::unique_ptr<Object3d>   pMonsterBall_           = nullptr;
    std::unique_ptr<Object3d>   pGrid_                  = nullptr;

    std::string                 name_ = "CG3";

    D3D12_VIEWPORT              viewport_               = {};


private:
    void DebugWindow();

};
