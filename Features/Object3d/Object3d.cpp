#include "Object3d.h"

#include "DirectX12.h"
#include "DX12Helper.h"
#include "Features/Model/Helper/ModelHelper.h"
#include <Win32Application.h>

#include <Matrix4x4.h>
#include <structs.h>
#include <TextureManager.h>

#ifdef DEBUG_ENGINE
#include <DebugTools/DebugManager/DebugManager.h>
#endif // DEBUG_ENGINE

const std::string Object3d::kDefaultDirectoryPath = "resources";

void Object3d::Initialize(Object3dSystem* _system, std::string _filepath)
{
    /// 必要なインスタンスを取得
    pDx12_ = _system->GetDx12();
    device_ = pDx12_->GetDevice();

#ifdef DEBUG_ENGINE
    pDebugManager_ = DebugManager::GetInstance();
    std::stringstream ss;
    ss << "instance##0x" << std::hex << this;
    pDebugManager_->SetComponent("Object3d", ss.str(), std::bind(&Object3d::DebugWindow, this));
#endif // DEBUG_ENGINE

    transform_.scale = Vector3(1.0f, 1.0f, 1.0f);
    transform_.rotate = Vector3(0.0f, 0.0f, 0.0f);
    transform_.translate = Vector3(0.0f, 0.0f, 0.0f);

    cameraTransform_.scale = Vector3(1.0f, 1.0f, 1.0f);
    cameraTransform_.rotate = Vector3(0.0f, 0.0f, 0.0f);
    cameraTransform_.translate = Vector3(0.0f, 0.0f, -10.0f);

    /// モデルデータを読み込む
    modelData_ = ModelHelper::LoadObjFile(kDefaultDirectoryPath, _filepath);

    /// 頂点リソースを作成
    CreateVertexResource();

    /// マテリアルリソースを作成
    CreateMaterialResource();

    /// 座標変換行列リソースを作成
    CreateTransformationMatrixResource();

    /// 平行光源リソースを作成
    CreateDirectionalLightResource();

    /// モデルのテクスチャを読み込む
    LoadModelTexture();
}

void Object3d::Update()
{
    Matrix4x4 wMatrix = Matrix4x4::AffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
    Matrix4x4 cameraMatrix = Matrix4x4::AffineMatrix({1.0f, 1.0f, 1.0f}, cameraTransform_.rotate, cameraTransform_.translate);
    Matrix4x4 vMatrix = cameraMatrix.Inverse();
    Matrix4x4 pMatrix = Matrix4x4::PerspectiveFovMatrix(0.45f, static_cast<float>(Win32Application::kClientWidth) / static_cast<float>(Win32Application::kClientHeight), 0.1f, 100.0f);

    transformationMatrixData_->wvp = wMatrix * (vMatrix * pMatrix);
    transformationMatrixData_->world = wMatrix;
}

void Object3d::Draw()
{
    ID3D12GraphicsCommandList* commandList = pDx12_->GetCommandList();
    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> textureSrvHandleGPUs = pDx12_->GetSRVHandlesGPUList();


    // 頂点バッファを設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    // マテリアルCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    // TransformationMatrixCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
    // SRVの設定
    commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);

    // 描画！（DrawCall/ドローコール）。頂点
    commandList->DrawInstanced(static_cast<uint32_t>(modelData_.vertices.size()), 1, 0, 0);
}

void Object3d::Finalize()
{
#ifdef DEBUG_ENGINE
    pDebugManager_->DeleteComponent("Object3d");
#endif // DEBUG_ENGINE
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

void Object3d::LoadModelTexture()
{
    TextureManager* textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture(modelData_.materialData.textureFilePath);
    modelData_.materialData.textureIndex = textureManager->GetTextureIndexByFilePath(modelData_.materialData.textureFilePath);
    textureSrvHandleGPU_ = TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.materialData.textureIndex);
}

#ifdef DEBUG_ENGINE
void Object3d::DebugWindow()
{
#ifdef _DEBUG
    ImGui::SeparatorText("Transform");
    ImGui::PushID("TRANSFORM");
    ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
    ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
    ImGui::DragFloat3("Translate", &transform_.translate.x, 0.01f);
    ImGui::PopID();

    ImGui::SeparatorText("Camera");
    ImGui::PushID("CAMERA");
    ImGui::DragFloat3("Rotate", &cameraTransform_.rotate.x, 0.01f);
    ImGui::DragFloat3("Translate", &cameraTransform_.translate.x, 0.01f);
    ImGui::PopID();
#endif // _DEBUG
}
#endif // DEBUG_ENGINE
