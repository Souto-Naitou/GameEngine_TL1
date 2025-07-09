#pragma once

#include <Scene/SceneBase.h>
#include <Features/Particle/Particle.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Input/Input.h>
#include <Features/Sprite/Sprite.h>

#include <memory>

class InstancingScene : public SceneBase
{
public:
    InstancingScene(ISceneArgs* pArgs_) : SceneBase(pArgs_) {}

    void Initialize() override;

    void Finalize() override;

    void Update() override;

    void Draw2dBackGround() override;

    void Draw3d() override;

    void DrawLine() override;

    void Draw2dForeground() override;

    void DrawTexts() override;

private:
    Particle* particle_ = nullptr;
    std::unique_ptr<GameEye> pGameEye_ = nullptr;
    std::unique_ptr<Sprite> pGuideSprite_ = nullptr;

    // Pointers
    Input* pInput_ = nullptr;
    ModelManager* pModelManager_ = nullptr;


private:
    void InitializeParticle();
};