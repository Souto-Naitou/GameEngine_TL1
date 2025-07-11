#pragma once

#include <Common/structs.h>
#include <dxcapi.h>
#include <Vector4.h>
#include <Vector3.h>
#include <Matrix4x4.h>
#include <d3d12.h> // ID3D12Resource
#include <map> // std::map
#include <list> // std::list

#include <BaseClasses/ObjectSystemBase.h>
#include <Features/Model/IModel.h>

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

// 3D object common 
class Object3dSystem : public ObjectSystemBase
{
public:
    struct CommandListData
    {
        std::map<UINT, ID3D12Resource*> cbuffers;
        IModel* model = nullptr;
    };

public:
    // For singleton instance
    Object3dSystem(const Object3dSystem&) = delete;
    Object3dSystem(Object3dSystem&&) = delete;
    Object3dSystem& operator=(const Object3dSystem&) = delete;
    Object3dSystem& operator=(Object3dSystem&&) = delete;

    static Object3dSystem* GetInstance()
    {
        static Object3dSystem instance;
        return &instance;
    }

    // Common function
    void    Initialize() override;
    void    DrawCall();
    void    Sync();

    // Draw settings
    void    DepthDrawSetting();
    void    MainDrawSetting();

    // Setter
    void    AddCommandListData(CommandListData& _data);

private:
    // Ctor
    Object3dSystem();

    // Internal functions
    void    CreateRootSignature();          // Create root signature
    void    CreateMainPipelineState();      // for main drawing
    void    CreateDepthPipelineState();     // for Early-Z

    // DirectX objects and paths
    static constexpr wchar_t        kVertexShaderPath[]     = L"EngineResources/Shaders/Object3d.VS.hlsl";
    static constexpr wchar_t        kPixelShaderPath[]      = L"EngineResources/Shaders/Object3d.PS.hlsl";
    ComPtr<ID3D12RootSignature>     rootSignature_          = nullptr;  // Root signature
    ComPtr<ID3D12PipelineState>     psoMain_                = nullptr;  // Pipeline state object for main drawing
    ComPtr<ID3D12PipelineState>     psoEarlyZ_              = nullptr;  // Pipeline state object for Early-Z
    ComPtr<IDxcBlob>                vertexShaderBlob_       = nullptr;  // Blob of vertex shader
    ComPtr<IDxcBlob>                pixelShaderBlob_        = nullptr;  // Blob of pixel shader
    std::list<CommandListData>      commandListDatas_       = {};       // Container for draw settings
    D3D12_INPUT_ELEMENT_DESC        inputElementDescs_[3]   = {};
    D3D12_INPUT_LAYOUT_DESC         inputLayoutDesc_        = {};
    D3D12_RASTERIZER_DESC           rasterizerDesc_         = {};
};