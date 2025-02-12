#pragma once

#include <Interfaces/IScene.h>
#include <Features/Object3d/Object3d.h>
#include <Features/Input/Input.h>
#include <Features/GameEye/GameEye.h>

#include <list>
#include <memory>

class DepthScene : public IScene
{
public:
    DepthScene() = default;
    ~DepthScene() = default;

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
    std::unique_ptr<GameEye> pGameEye_ = nullptr;
    std::list<std::unique_ptr<Object3d>> objectList_;
    float nextObjZ_ = 0;
    const float objZInterval_ = 0.01f;
    unsigned int nextCreateCount_ = 0;
    int nextCreateCountTemp_ = 0;

private:
    Input* pInput_ = nullptr;

private:
    void CreateObject();
    void DebugWindow();
    std::string windowName_ = "DepthScene";
};
