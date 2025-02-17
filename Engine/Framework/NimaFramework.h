#pragma once

#include <DebugTools/Logger/Logger.h>
#include <Features/Audio/AudioManager.h>
#include <Features/Input/Input.h>
#include <Core/DirectX12/TextureManager.h>
#include <DebugTools/DebugManager/DebugManager.h>
#include <DebugTools/ImGuiManager/ImGuiManager.h>
#include <Features/Sprite/SpriteSystem.h>
#include <Features/Object3d/Object3dSystem.h>
#include <Features/Particle/ParticleSystem.h>
#include <Features/Model/ModelManager.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/Win32/WinSystem.h>
#include <Core/DirectX12/SRVManager.h>
#include <Features/SceneManager/SceneManager.h>
#include <Interfaces/ISceneFactory.h>
#include <Features/Particle/ParticleManager.h>
#include <Features/Line/LineSystem.h>
#include <Features/RandomGenerator/RandomGenerator.h>
#include <Features/Line/Line.h>
#include <Features/Text/TextSystem.h>
#include <Features/Viewport/Viewport.h>

#include <memory>

// ゲーム共有
class NimaFramework
{
public:
    void Run();


public:
    virtual ~NimaFramework() {}

    virtual void Initialize();
    virtual void Finalize();
    virtual void Update();
    virtual void Draw() = 0;

    virtual bool IsExitProgram() const { return isExitProgram_; }


protected: /// システムクラスのインスタンス
    std::unique_ptr<ISceneFactory> pSceneFactory_ = nullptr;
    std::unique_ptr<ImGuiManager> pImGuiManager_;
    std::unique_ptr<Viewport> pViewport_;


protected: /// 他クラスのインスタンス
    Logger* pLogger_;
    DirectX12* pDirectX_;
    DebugManager* pDebugManager_;
    WinSystem* pWinSystem_;
    ModelManager* pModelManager_;
    SRVManager* pSRVManager_;
    TextureManager* pTextureManager_;
    SceneManager* pSceneManager_;
    SpriteSystem* pSpriteSystem_;
    Object3dSystem* pObject3dSystem_;
    ParticleSystem* pParticleSystem_;
    ParticleManager* pParticleManager_;
    LineSystem* pLineSystem_;
    TextSystem* pTextSystem_;
    Input* pInput_;
    RandomGenerator* pRandomGenerator_;
    AudioManager* pAudioManager_;

protected:
    bool isExitProgram_ = false;
};

#define CREATE_APPLICATION(class) \
int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) \
{ \
    std::unique_ptr<NimaFramework> program = std::make_unique<class>(); \
    program->Run(); \
    return 0; \
}

#define CREATE_FRAMEWORK(classname) \
\
class classname : public NimaFramework \
{ \
public: \
    void Initialize() override; \
    void Finalize() override; \
    void Update() override; \
    void Draw() override; \
    bool IsExitProgram() { return isExitProgram_; } \
private: \
};