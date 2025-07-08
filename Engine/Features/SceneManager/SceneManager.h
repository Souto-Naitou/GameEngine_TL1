#pragma once

#include <Scene/SceneBase.h>
#include <Interfaces/ISceneFactory.h>
#include <Features/SceneTransition/SceneTransitionManager.h>
#include <Features/Model/ModelManager.h>
#include <memory>
#include <Interfaces/ISceneArgs.h>

class SceneManager
{
public:
    SceneManager(SceneManager const&) = delete;
    void operator=(SceneManager const&) = delete;
    SceneManager(SceneManager&&) = delete;
    void operator=(SceneManager&&) = delete;

    static SceneManager* GetInstance()
    {
        static SceneManager instance;
        return &instance;
    }


public: /// Setter
    void SetSceneFactory(ISceneFactory* _pSceneFactory);
    void SetSceneArgs(std::unique_ptr<ISceneArgs> _pSceneArgs);
    void SetModelManager(ModelManager* _pModelManager);


public:
    void ReserveScene(const std::string& _sceneName);
    void ReserveStartupScene();


public: /// シーン動作
    void Initialize();
    void Update();
    void SceneDraw2dBackGround();
    void SceneDraw3d();
    void SceneDrawLine();
    void SceneDraw2dForeground();
    void SceneDrawText();
    void Finalize();


private:
    SceneManager() = default;
    void ChangeScene();
    void DebugWindow();
    void PackSceneArgs();

    std::string name_ = "SceneManager";
    char buffer[128] = {};

    bool isReserveScene_ = false;

    std::string nextSceneName_;
    std::unique_ptr<SceneBase> pCurrentScene_ = nullptr;
    std::unique_ptr<ISceneArgs> pSceneArgs_ = nullptr;


private: /// 他クラスのインスタンス
    ISceneFactory* pSceneFactory_ = nullptr;
    SceneTransitionManager* pSceneTransitionManager_ = nullptr;
    ModelManager* pModelManager_ = nullptr;
};