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

struct VertexData
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

enum class LightingType : int32_t
{
	LambertianReflectance,
	HarfLambert
};

struct Material
{
	Vector4 color;
	LightingType lightingType;
	int32_t enableLighting;
	float padding[2];
	Matrix4x4 uvTransform;
};

struct TransformationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
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

struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData materialData;
};

struct D3DResourceLeakChecker
{
	~D3DResourceLeakChecker()
	{
		// リソースリークチェック
		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
		{
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

struct ImGuiListData
{
	std::vector<const char*> label;
	unsigned int numIndex;
};

struct ModelScene
{
	ModelData modelData;
	Material* material;
	unsigned int selectedTextureIndex; // numIndexを保存しておく用
};