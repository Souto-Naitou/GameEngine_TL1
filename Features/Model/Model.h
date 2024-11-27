#pragma once

#include <Common/structs.h>
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <DirectX12.h>


struct Material;
struct VertexData;

class Model
{
public: /// 公開メンバ関数
    void Initialize(const std::string& _filePath);
    void Update();
    void Draw();


private: /// メンバ変数
    static const std::string kDefaultDirectoryPath;

    ModelData                               modelData_                  = {};
    Microsoft::WRL::ComPtr<ID3D12Resource>  vertexResource_             = nullptr;
    D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_           = {};
    VertexData*                             vertexData_                 = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>  materialResource_           = nullptr;
    Material*                               materialData_               = nullptr;
    D3D12_GPU_DESCRIPTOR_HANDLE             textureSrvHandleGPU_        = {};


private: /// 非公開メンバ関数
    void CreateMaterialResource();
    void CreateVertexResource();
    void LoadModelTexture();

private: /// 他クラスのインスタンス
    DirectX12* pDx12_ = nullptr;
    ID3D12Device* device_ = nullptr;
};