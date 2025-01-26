#pragma once

#include "Object3dSystem.h"

#include <wrl.h>
#include <d3d12.h>
#include <string>
#include <Common/structs.h>
#include <Features/Model/Model.h>
#include <Common/define.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Lighting/PointLight/PointLight.h>

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
    void Initialize(const std::string& _filePath);

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
    const Vector3& GetScale() const                             { return transform_.scale; }
    const Vector3& GetRotate() const                            { return transform_.rotate; }
    const Vector3& GetTranslate() const                         { return transform_.translate; }
    const Matrix4x4& GetRotateMatrix() const                    { return rotateMatrix_; }


public: /// Setter
    void SetScale(const Vector3& _scale)                        { transform_.scale = _scale; }
    void SetRotate(const Vector3& _rotate)                      { transform_.rotate = _rotate; }
    void SetTranslate(const Vector3& _translate)                { transform_.translate = _translate; }
    void SetModel(Model* _pModel)                               { pModel_ = _pModel; }
    void SetGameEye(GameEye* _pGameEye)                         { pGameEye_ = _pGameEye; }
    void SetName(const std::string& _name)                      { name_ = _name; }
    void SetTilingMultiply(const Vector2& _tilingMultiply)      { tilingData_->tilingMultiply = _tilingMultiply; }
    void SetDirectionalLight(DirectionalLight* _light)          { directionalLight_ = _light; }
    void SetEnableLighting(bool _flag)                          { lightingData_->enableLighting = _flag; }
    void SetLightingType(LightingType _type)                    { lightingData_->lightingType = _type; }
    void SetPointLight(PointLight* _light)                      { pointLight_ = _light; }
    void SetShininess(float _shininess)                         { materialData_->shininess = _shininess; }


private: /// メンバ変数
    Transform                                       transform_                      = {};
    Matrix4x4                                       rotateMatrix_                   = {};
    std::string                                     name_                           = {};

    bool                                            isUpdate_                       = true;
    bool                                            isDraw_                         = true;

    Microsoft::WRL::ComPtr<ID3D12Resource>          transformationMatrixResource_   = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>          directionalLightResource_       = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>          tilingResource_                 = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>          cameraForGPUResource_           = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>          lightingResource_               = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>          pointLightResource_             = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>          materialResource_               = nullptr;

    TransformationMatrix*                           transformationMatrixData_       = nullptr;
    DirectionalLight*                               directionalLight_               = nullptr;
    DirectionalLight*                               directionalLightData_           = nullptr;
    PointLight*                                     pointLight_                     = nullptr;
    PointLight*                                     pointLightData_                 = nullptr;
    TilingData*                                     tilingData_                     = nullptr;
    CameraForGPU*                                   cameraForGPU_                   = nullptr;
    Lighting*                                       lightingData_                   = nullptr;
    Material*                                       materialData_                   = nullptr;

    bool                                            isEnableLighting_               = true;

    Model*                                          pModel_                         = nullptr;
    std::string                                     modelPath_                      = {};
    GameEye*                                        pGameEye_                       = nullptr;
    GameEye**                                       ppSystemGameEye_                = nullptr;

private: /// 非公開メンバ関数
    void CreateTransformationMatrixResource();
    void CreateDirectionalLightResource();
    void CreateTilingResource();
    void CreateCameraForGPUResource();
    void CreateLightingResource();
    void CreatePointLightResource();
    void CreateMaterialResource();

#ifdef DEBUG_ENGINE
    void DebugWindow();
#endif // DEBUG_ENGINE


private: /// 他クラスが所持するインスタンスへのポインタ
    DirectX12*      pDx12_          = nullptr;
    ID3D12Device*   device_         = nullptr;
    Object3dSystem* pSystem_        = nullptr;

#ifdef DEBUG_ENGINE
    DebugManager*   pDebugManager_  = nullptr;
#endif // DEBUG_ENGINE
};