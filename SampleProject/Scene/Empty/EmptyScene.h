#pragma once

#include <Scene/SceneBase.h>

class EmptyScene : public SceneBase
{
public:
    EmptyScene() = default;
    ~EmptyScene() = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw2dBackGround() override;
    void Draw3d() override;
    void DrawLine() override;
    void Draw2dForeground() override;
    void DrawTexts() override;

private:
};
