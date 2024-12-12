#pragma once

#include <Features/Input/Input.h>
#include <Interfaces/IScene.h>
#include <Features/Object3d/Object3d.h>
#include <Features/Model/Model.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Particle/Particle.h>
#include <Features/Particle/Emitter/ParticleEmitter.h>
#include <Features/Sprite/Sprite.h>

class GameScene : public IScene
{
public:
    void Initialize() override;

    void Finalize() override;

    void Update() override;

    void Draw2dBackGround() override;

    void Draw3d() override;

    void Draw2dForeground() override;

private:
    GameEye* pGameEye_;
    ParticleEmitter* pSmoke_;
    ParticleEmitter* pFirework_;
    ParticleEmitter* pSpark_;
    Object3d* pSkydome_;
    Object3d* pGrid_;
    Sprite* pGuideSprite_;

private:
    Input* pInput_;


private:
    void EmitterSetting();

};