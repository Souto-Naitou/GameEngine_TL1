#pragma once

#include <wrl.h>
#include <d3d12.h>
#include "Object3dSystem.h"
#include <string>
#include <define.h>

/// 前方宣言
struct ModelData;
struct VertexData;
struct DirectionalLight;

class DirectX12;

#ifdef DEBUG_ENGINE
class DebugManager;
#endif // DEBUG_ENGINE

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(Object3dSystem* _system, std::string _filepath);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize();

public: /// Setter
    void SetScale(const Vector3& _scale) { transform_.scale = _scale; }
    void SetRotate(const Vector3& _rotate) { transform_.rotate = _rotate; }
    void SetTranslate(const Vector3& _translate) { transform_.translate = _translate; }

private:
    static const std::string kDefaultDirectoryPath;

    Transform transform_ = {};
    Transform cameraTransform_ = {};

    ModelData modelData_ = {};

    Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
    VertexData* vertexData_ = nullptr;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};

    Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
    Material* materialData_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;
    TransformationMatrix* transformationMatrixData_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
    DirectionalLight* directionalLight_ = nullptr;

    D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_ = {};

private:
    void CreateVertexResource();
    void CreateMaterialResource();
    void CreateTransformationMatrixResource();
    void CreateDirectionalLightResource();
    void LoadModelTexture();

#ifdef DEBUG_ENGINE
    void DebugWindow();
#endif // DEBUG_ENGINE

private: /// 他クラスが所持するインスタンスへのポインタ
    DirectX12* pDx12_ = nullptr;
    ID3D12Device* device_ = nullptr;

#ifdef DEBUG_ENGINE
    DebugManager* pDebugManager_ = nullptr;
#endif // DEBUG_ENGINE
};