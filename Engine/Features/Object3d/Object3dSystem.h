#pragma once

#include <Common/structs.h>
#include <Core/DirectX12/DirectX12.h>
#include <Features/GameEye/GameEye.h>
#include <Features/Model/Model.h>
#include <Vector4.h>
#include <Vector3.h>
#include <d3d12.h> /// ID3D12Resource
#include <vector> /// std::vector
#include <future> /// std::future
#include <map> /// std::map
#include <list> /// std::list

#include <BaseClasses/ObjectSystemBase.h>

struct Material
{
    Vector4 color;
    Matrix4x4 uvTransform;
    float shininess;
};

struct Lighting
{
    int32_t enableLighting;
    LightingType lightingType;
};

struct CameraForGPU
{
    Vector3 worldPosition;
};

/// <summary>
///  3Dオブジェクトシステム
/// </summary>
class Object3dSystem : public ObjectSystemBase
{
public:
    struct CommandListData
    {
        std::map<UINT, ID3D12Resource*> cbuffers;
        Model* model;
    };

public:
    Object3dSystem(const Object3dSystem&) = delete;
    Object3dSystem& operator=(const Object3dSystem&) = delete;
    Object3dSystem(Object3dSystem&&) = delete;
    Object3dSystem& operator=(Object3dSystem&&) = delete;

    static Object3dSystem* GetInstance()
    {
        static Object3dSystem instance;
        return &instance;
    }

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    void DepthDrawSetting();
    void MainDrawSetting();

    void DrawCall();
    void Sync();


public: /// Getter
    DirectX12*  GetDx12()                       { return pDx12_; }
    GameEye**   GetSharedGameEye()              { return &pDefaultGameEye_; }
    ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }


public: /// Setter
    void SetSharedGameEye(GameEye* _pGameEye)   { pDefaultGameEye_ = _pGameEye; }
    void AddCommandListData(CommandListData& _data) { commandListDatas_.emplace_back(_data); }


private:
    Object3dSystem();

    void CreateRootSignature();
    void CreateMainPipelineState();
    void CreateDepthPipelineState();


private: /// メンバ変数
    static constexpr wchar_t kVertexShaderPath[] = L"EngineResources/Shaders/Object3d.VS.hlsl";
    static constexpr wchar_t kPixelShaderPath[] = L"EngineResources/Shaders/Object3d.PS.hlsl";
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> psoMain_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> psoEarlyZ_ = nullptr;
    Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
    Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;

    D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3] = {};
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_ = {};
    D3D12_RASTERIZER_DESC rasterizerDesc_ = {};

    std::list<CommandListData> commandListDatas_;


private:
    GameEye*    pDefaultGameEye_    = nullptr;
};