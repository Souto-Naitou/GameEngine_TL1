#pragma once

#include "./IModel.h"

#include <Common/structs.h>
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <Core/DirectX12/DirectX12.h>
#include <thread>
#include <memory>


struct Material;
struct VertexData;

class Model : 
    public IModel,
    public EngineFeature
{
public:
    // Common functions
    ~Model();
    void    Initialize() override;
    void    Update() override;
    void    Draw() override;
    void    Initialize(const std::string& _filePath);
    void    Initialize(const std::string& _modelPath, const std::string& _texturePath);
    void    Upload();

    // Getter
    ModelData*                  GetModelData();
    D3D12_VERTEX_BUFFER_VIEW    GetVertexBufferView() const;
    bool                        IsUploaded() const;
    D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() const;

    // Setter
    int ChangeTexture(const std::string& _filepath);


private: /// メンバ変数
    static const std::string kDefaultDirectoryPath;

    ModelData                               modelData_                  = {};
    Microsoft::WRL::ComPtr<ID3D12Resource>  vertexResource_             = nullptr;
    D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_           = {};
    VertexData*                             vertexData_                 = nullptr;
    D3D12_GPU_DESCRIPTOR_HANDLE             textureSrvHandleGPU_        = {};
    std::string                             texturePath_                = {};

    bool                                    isUploaded_                 = false;


private: /// 非公開メンバ関数
    void CreateVertexResource();
    void LoadModelTexture();

private: /// 他クラスのインスタンス
    ID3D12Device* device_ = nullptr;
    std::unique_ptr<std::thread> th_LoadObjectFile_ = nullptr;
    std::string filePath_;
    std::string directoryPath_;
};