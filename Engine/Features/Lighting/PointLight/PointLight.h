#pragma once
#include <memory>

#include <Common/structs.h>

#include <Vector4.h>
#include <Vector3.h>

class Object3d;

class PointLight
{
public:
    PointLight() = default;
    ~PointLight() = default;

    void Initialize();
    void Update();
    void Draw();
    void Finalize();

    int enablePointLight = 0; //!< ポイントライトの有効無効
    float padding[3] = {};
    Vector4 color = {}; //!< ライトの色
    Vector3 position = {}; //!< ライトの位置
    float intensity = 0.0f; //!< 輝度
private:
    std::unique_ptr<Object3d> pIcon_ = nullptr;
};