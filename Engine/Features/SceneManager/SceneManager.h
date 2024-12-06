#pragma once

#include <Interfaces/IScene.h>
#include <memory>
#include <Interfaces/ISceneFactory.h>

class SceneManager
{
public:
    SceneManager(SceneManager const&) = delete;
    void operator=(SceneManager const&) = delete;
    SceneManager(SceneManager&&) = delete;
    void operator=(SceneManager&&) = delete;

    static SceneManager& GetInstance()
    {
        static SceneManager instance;
        return instance;
    }


public: /// Setter
    void SetSceneFactory(ISceneFactory* _pSceneFactory) { pSceneFactory_ = _pSceneFactory; }


public:
    void ReserveScene(const std::string& _sceneName);

public: /// シーン動作
    void Update();
    void SceneDraw2dBackGround();
    void SceneDraw3d();
    void SceneDraw2dForeground();
    void Finalize();


private:
    SceneManager() = default;
    void ChangeScene();

    bool isReserveScene_ = false;

    std::string nextSceneName_;
    IScene* pCurrentScene_ = nullptr;

private: /// 他クラスのインスタンス
    ISceneFactory* pSceneFactory_ = nullptr;
};