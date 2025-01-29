#pragma once

#include <Interfaces/IScene.h>
#include <Features/Particle/Particle.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Input/Input.h>
#include <Features/Sprite/Sprite.h>

class InstancingScene : public IScene
{
public:
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
    Particle* particle_ = nullptr;
    GameEye* pGameEye_ = nullptr;
    Sprite* pGuideSprite_ = nullptr;


private: /// 他クラスのインスタンス
    Input* pInput_ = nullptr;


private:
    void InitializeParticle();
};