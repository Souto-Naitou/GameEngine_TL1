#pragma once
#include <Features/Model/ModelManager.h>
#include <Interfaces/ISceneArgs.h>

class SceneBase
{
public:
    /// dtor
    SceneBase(ISceneArgs* _pArgs);
    virtual ~SceneBase() {}

    /// <summary>
    /// 初期化
    /// </summary>
    virtual void Initialize() = 0;

    /// <summary>
    /// 終了
    /// </summary>
    virtual void Finalize() = 0;

    /// <summary>
    /// 更新
    /// </summary>
    virtual void Update() = 0;

    /// <summary>
    /// 背景描画
    /// </summary>
    virtual void Draw2dBackGround() = 0;

    /// <summary>
    /// 3D描画
    /// </summary>
    virtual void Draw3d() = 0;

    /// <summary>
    /// ライン描画
    /// </summary>
    virtual void DrawLine() = 0;

    /// <summary>
    /// 前景描画
    /// </summary>
    virtual void Draw2dForeground() = 0;

    /// <summary>
    /// テキスト描画
    /// </summary>
    virtual void DrawTexts() = 0;

protected:
    ISceneArgs* pArgs_ = nullptr;
};