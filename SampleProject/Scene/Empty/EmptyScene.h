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
    void Draw() override;
    void DrawTexts() override;

private:
};
