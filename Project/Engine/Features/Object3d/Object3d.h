#pragma once

#include "Object3dSystem.h"

#include <wrl.h>
#include <d3d12.h>
#include <string>
#include <Common/structs.h>
#include <Features/Model/Model.h>

/// 前方宣言
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
    void Initialize(Object3dSystem* _system);

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

public: /// Getter
    const Vector3& GetScale() const { return transform_.scale; }
    const Vector3& GetRotate() const { return transform_.rotate; }
    const Vector3& GetTranslate() const { return transform_.translate; }


public: /// Setter
    void SetScale(const Vector3& _scale) { transform_.scale = _scale; }
    void SetRotate(const Vector3& _rotate) { transform_.rotate = _rotate; }
    void SetTranslate(const Vector3& _translate) { transform_.translate = _translate; }
    void SetModel(Model* _pModel) { pModel_ = _pModel; }

private:
    Transform                                   transform_                      = {};
    Transform                                   cameraTransform_                = {};

    Microsoft::WRL::ComPtr<ID3D12Resource>      transformationMatrixResource_   = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>      directionalLightResource_       = nullptr;

    TransformationMatrix*                       transformationMatrixData_       = nullptr;
    DirectionalLight*                           directionalLight_               = nullptr;

private:
    void CreateTransformationMatrixResource();
    void CreateDirectionalLightResource();

#ifdef DEBUG_ENGINE
    void DebugWindow();
#endif // DEBUG_ENGINE


private: /// 他クラスが所持するインスタンスへのポインタ
    DirectX12*      pDx12_          = nullptr;
    ID3D12Device*   device_         = nullptr;
    Model*          pModel_         = nullptr;

#ifdef DEBUG_ENGINE
    DebugManager*   pDebugManager_  = nullptr;
#endif // DEBUG_ENGINE
};