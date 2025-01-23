#include "SceneManager.h"
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
    if (!pSceneTransitionManager_) pSceneTransitionManager_ = SceneTransitionManager::GetInstance();

    if (isReserveScene_)
    {
        ChangeScene();
        isReserveScene_ = false;
    }

    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Update();
    }

    pSceneTransitionManager_->Update();
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

void SceneManager::SceneDraw2dMidground()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw2dMidground();
    }
}

void SceneManager::SceneDraw3dMidground()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->Draw3dMidground();
    }
}

void SceneManager::SceneDrawLine()
{
    if (pCurrentScene_ != nullptr)
    {
        pCurrentScene_->DrawLine();
    }
}

void SceneManager::SceneDraw2dForeground()
{
    pSceneTransitionManager_->Draw();

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
        delete pCurrentScene_;
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
