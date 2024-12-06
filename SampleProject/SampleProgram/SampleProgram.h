#pragma once

#include <Framework/NimaFramework.h>

#include <Features/Sprite/Sprite.h>
#include <Features/Object3d/Object3d.h>
#include <Features/Model/Model.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Particle/Particle.h>


class SampleProgram : public NimaFramework
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

    bool IsExitProgram() { return isExitProgram_; }


private: /// ゲーム内オブジェクト
    Object3d* pObject3d_;
    GameEye* pGameEye_;
    Sprite* pSpriteMB_;
    Sprite* pSpriteUVC_;
    Particle* pParticle_;
};