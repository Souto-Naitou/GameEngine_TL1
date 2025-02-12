#pragma once

class IScene
{
public:
    /// デストラクタ
    IScene() {}
    virtual ~IScene() {}

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
    /// 中景描画
    /// </summary>
    virtual void Draw2dMidground() = 0;

    /// <summary>
    /// 3D中景描画
    /// </summary>
    virtual void Draw3dMidground() = 0;

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
};

#define NEW_SCENE_IMPL(class)\
void class::Initialize()\
{\
}\
\
void class::Finalize()\
{\
}\
\
void class::Update()\
{\
}\
\
void class::Draw2dBackGround()\
{\
}\
\
void class::Draw3d()\
{\
}\
\
void class::Draw2dMidground()\
{\
}\
\
void class::Draw3dMidground()\
{\
}\
\
void class::DrawLine()\
{\
}\
\
void class::Draw2dForeground()\
{\
}\
\
void class::DrawTexts()\
{\
}\
