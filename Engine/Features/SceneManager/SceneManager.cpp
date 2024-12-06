#include "SceneManager.h"
#include <cassert>

void SceneManager::ReserveScene(const std::string& _name)
{
    isReserveScene_ = true;
    nextSceneName_ = _name;

    return;
}

void SceneManager::Update()
{
    if (isReserveScene_)
    {
        ChangeScene();
        isReserveScene_ = false;
    }

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

void SceneManager::ChangeScene()
{
    assert(pSceneFactory_);

    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Finalize();
        delete pCurrentScene_;
    }

    pCurrentScene_ = pSceneFactory_->CreateScene(nextSceneName_);
    pCurrentScene_->Initialize();
}
