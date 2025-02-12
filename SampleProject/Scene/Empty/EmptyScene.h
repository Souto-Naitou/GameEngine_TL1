#pragma once

#include <Interfaces/IScene.h>

class EmptyScene : public IScene
{
public:
    EmptyScene() = default;
    ~EmptyScene() = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw2dBackGround() override;
    void Draw3d() override;
    void Draw2dMidground() override;
    void Draw3dMidground() override;
    void DrawLine() override;
    void Draw2dForeground() override;
    void DrawTexts() override;

private:
};
