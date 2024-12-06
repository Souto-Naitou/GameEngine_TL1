#include "D:\VSProject\CG\GameEngine_DX12\Engine\Common\stdafx.h"
#include "SceneManager.h"

void SceneManager::ChangeScene(std::unique_ptr<IScene> _scene)
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Finalize();
    }
    pCurrentScene_ = std::move(_scene);
    pCurrentScene_->Initialize();

    return;
}

void SceneManager::SceneUpdate()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Update();
    }
}

void SceneManager::SceneDraw2dBackGround()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw2dBackGround();
    }
}

void SceneManager::SceneDraw3d()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw3d();
    }
}

void SceneManager::SceneDraw2dForeground()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw2dForeground();
    }
}

void SceneManager::Finalize()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Finalize();
    }
}
