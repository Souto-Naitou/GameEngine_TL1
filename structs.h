#pragma once
#include "math/Vector4.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "Matrix4x4.h"

#include <vector>
#include <string>
#include <dxgidebug.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>


enum class LightingType : int32_t
{
	LambertianReflectance,
	HarfLambert
};


struct DirectionalLight
{
	Vector4 color; //!< ライトの色
	Vector3 direction; //!< ライトの向き (正規化必須)
	float intensity; //!< 輝度
};

struct MaterialData
{
	std::string textureFilePath;
	Vector4 diffuse;
};

struct Transform
{
	Vector3 scale;
    Vector3 rotate;
    Vector3 translate;
};