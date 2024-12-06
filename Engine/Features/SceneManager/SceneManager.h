#pragma once

#include <Interfaces/IScene.h>
#include <memory>

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


public:
    void ChangeScene(std::unique_ptr<IScene> _scene);


public: /// シーン動作
    void SceneUpdate();
    void SceneDraw2dBackGround();
    void SceneDraw3d();
    void SceneDraw2dForeground();
    void Finalize();


private:
    SceneManager() = default;

    std::unique_ptr<IScene> pCurrentScene_ = nullptr;
};