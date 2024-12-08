#pragma once

#include <Interfaces/IScene.h>
#include <Features/Sprite/Sprite.h>
#include <Features/Object3d/Object3d.h>
#include <Features/Model/Model.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Particle/Particle.h>
#include <Features/Sprite/SpriteSystem.h>
#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/Particle/Emitter/ParticleEmitter.h>
#include <Features/Line/LineSystem.h>
#include <Features/Line/Line.h>

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
    Object3d* pObject3d_;
    GameEye* pGameEye_;
    Sprite* pSpriteMB_;
    Sprite* pSpriteUVC_;
    ParticleEmitter* pParticleEmitter_;
    Object3d* pSkydome_;
    Object3d* pGrid_;
    Line* pLine_;

private:
    ParticleSystem* pParticleSystem_ = nullptr;
    LineSystem* pLineSystem_ = nullptr;
};