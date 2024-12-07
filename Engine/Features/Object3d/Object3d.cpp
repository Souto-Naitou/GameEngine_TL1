#include "Object3d.h"

#include <Matrix4x4.h>
#include <Common/structs.h>
#include <Core/Win32/Win32Application.h>
#include <Core/DirectX12/DirectX12.h>
#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Core/DirectX12/TextureManager.h>
#include <Features/Model/ModelManager.h>


#if defined (DEBUG_ENGINE) && (_DEBUG)
#include <DebugTools/DebugManager/DebugManager.h>
#include <imgui.h>
#endif // DEBUG_ENGINE && _DEBUG


void Object3d::Initialize(const std::string& _filePath)
{
    /// 必要なインスタンスを取得
    pSystem_ = Object3dSystem::GetInstance();
    pDx12_ = pSystem_->GetDx12();
    device_ = pDx12_->GetDevice();
    pGameEye_ = pSystem_->GetDefaultGameEye();

#if defined (DEBUG_ENGINE) && (_DEBUG)
    pDebugManager_ = DebugManager::GetInstance();
    std::stringstream ss;
    ss << "instance##0x" << std::hex << this;
    name_ = ss.str();
    pDebugManager_->SetComponent("Object3d", name_, std::bind(&Object3d::DebugWindow, this));
#endif // DEBUG_ENGINE && _DEBUG

    transform_.scale = Vector3(1.0f, 1.0f, 1.0f);
    transform_.rotate = Vector3(0.0f, 0.0f, 0.0f);
    transform_.translate = Vector3(0.0f, 0.0f, 0.0f);

    /// 座標変換行列リソースを作成
    CreateTransformationMatrixResource();

    /// 平行光源リソースを作成
    CreateDirectionalLightResource();

    /// モデルを読み込む
    modelPath_ = _filePath;
    ModelManager::GetInstance()->LoadModel(modelPath_);
    pModel_ = ModelManager::GetInstance()->FindModel(modelPath_);
}

void Object3d::Update()
{
    if (!isUpdate_) return;

    /// モデルが読み込まれていない場合は読み込む
    if (!pModel_) pModel_ = ModelManager::GetInstance()->FindModel(modelPath_);

    Matrix4x4 wMatrix = Matrix4x4::AffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
    Matrix4x4 wvpMatrix = {};

    /// カメラの行列計算
    if (pGameEye_)
    {
        auto& vpMatrix = pGameEye_->GetViewProjectionMatrix();
        wvpMatrix = wMatrix * vpMatrix;
    }
    else wvpMatrix = wMatrix;

    /// 座標変換行列データを更新
    transformationMatrixData_->wvp = wvpMatrix;
    transformationMatrixData_->world = wMatrix;

    /// 平行光源の方向を正規化
    directionalLight_->direction = directionalLight_->direction.Normalize();

    if (pModel_) pModel_->Update();
}

void Object3d::Draw()
{
    if (!isDraw_) return;

    ID3D12GraphicsCommandList* commandList = pDx12_->GetCommandList();
    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> textureSrvHandleGPUs = pDx12_->GetSRVHandlesGPUList();

    // TransformationMatrixCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
    // 平行光源CBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

    if (pModel_) pModel_->Draw();
}

void Object3d::Finalize()
{
#if defined (DEBUG_ENGINE) && (_DEBUG)
    pDebugManager_->DeleteComponent("Object3d");
#endif // DEBUG_ENGINE && _DEBUG
}


void Object3d::CreateTransformationMatrixResource()
{
    /// 座標変換行列リソースを作成
    transformationMatrixResource_ = DX12Helper::CreateBufferResource(device_, sizeof(TransformationMatrix));
    transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
    /// 座標変換行列データを初期化
    transformationMatrixData_->wvp = Matrix4x4::Identity();
    transformationMatrixData_->world = Matrix4x4::Identity();
}

void Object3d::CreateDirectionalLightResource()
{
    /// 平行光源リソースを作成
    directionalLightResource_ = DX12Helper::CreateBufferResource(device_, sizeof(DirectionalLight));
    directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight_));
    /// 平行光源データを初期化
    directionalLight_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    directionalLight_->direction = Vector3(0.0f, -1.0f, 0.0f);
    directionalLight_->intensity = 1.0f;
}

#ifdef DEBUG_ENGINE
void Object3d::DebugWindow()
{
#ifdef _DEBUG

    ImGui::Checkbox("Update", &isUpdate_);
    ImGui::SameLine();
    ImGui::Checkbox("Draw", &isDraw_);

    ImGui::SeparatorText("Transform");
    ImGui::PushID("TRANSFORM");
    ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
    ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
    ImGui::DragFloat3("Translate", &transform_.translate.x, 0.01f);
    ImGui::PopID();

    ImGui::SeparatorText("Directional Light");
    ImGui::PushID("DIRECTIONAL_LIGHT");
    ImGui::ColorEdit4("Color", &directionalLight_->color.x);
    ImGui::DragFloat3("Direction", &directionalLight_->direction.x, 0.01f);
    ImGui::DragFloat("Intensity", &directionalLight_->intensity, 0.01f);
    ImGui::PopID();

#endif // _DEBUG
}
#endif // DEBUG_ENGINE
