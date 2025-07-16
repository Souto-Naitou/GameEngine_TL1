#pragma once

#include <Features/Input/Input.h>
#include <Scene/SceneBase.h>
#include <Features/Object3d/Object3d.h>
#include <Features/GameEye/GameEye.h>


#include <memory>
#include <Features/Particle/Emitter/ParticleEmitter.h>
#include <Features/Text/Text.h>
#include <Features/Model/IModel.h>
#include <Features/Model/ModelManager.h>
#include <Interfaces/ISceneArgs.h>

class CG4Task1 : public SceneBase
{
public:
    /// Common function
    CG4Task1(ISceneArgs* pArgs_) : SceneBase(pArgs_) {}
    void    Initialize() override;
    void    Finalize() override;
    void    Update() override;
    void    Draw() override;
    void    DrawTexts() override;

private:
    // Resources
    std::unique_ptr<GameEye>            pGameEye_           = nullptr;
    std::unique_ptr<Object3d>           pGrid_              = nullptr;
    std::unique_ptr<IModel>             pModelSpark_        = nullptr;
    std::unique_ptr<IModel>             pModelCircle_       = nullptr;
    std::unique_ptr<IModel>             pModelGrid_         = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Basic_     = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Stars_     = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Rain_      = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Snow_      = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Spark_     = nullptr;
    std::unique_ptr<ParticleEmitter>    pEmitter_Test_      = nullptr;
    std::unique_ptr<Text>               pText_              = nullptr;

    // Pointers
    Input*  pInput_     = nullptr;
    ModelManager* pModelManager_ = nullptr;
};