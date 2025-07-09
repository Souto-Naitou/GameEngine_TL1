#pragma once

#include "./IModel.h"

#include <Common/structs.h>
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <Core/DirectX12/DirectX12.h>


struct Material;
struct VertexData;

class ModelFromObj : 
    public IModel,
    public EngineFeature
{
public:
    // Common functions
    ~ModelFromObj();
    void    Initialize() override;
    void    Update() override;
    void    Draw() override;
    void    CreateGPUResource() override;
    void    Clone(IModel* _src) override;

    // Getter
    ModelData*                  GetModelData();
    size_t                      GetVertexCount() const override;
    D3D12_VERTEX_BUFFER_VIEW    GetVertexBufferView() const override;
    D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandle() const override;
    bool                        IsEndLoading() const;

    // Setter
    int ChangeTexture(const std::string& _filepath);
    void SetCommandList(ID3D12GraphicsCommandList* _cl);
    void ChangeTexture(D3D12_GPU_DESCRIPTOR_HANDLE _texSrvHnd) override;


private: /// メンバ変数
    static const std::string kDefaultDirectoryPath;

    ID3D12GraphicsCommandList*              commandList_ = nullptr;
    ModelData                               modelData_                  = {};
    Microsoft::WRL::ComPtr<ID3D12Resource>  vertexResource_             = nullptr;
    D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_           = {};
    VertexData*                             vertexData_                 = nullptr;
    D3D12_GPU_DESCRIPTOR_HANDLE             textureSrvHandleGPU_        = {};
    std::string                             texturePath_                = {};
    bool                                    isReadyDraw_                = false;

    ModelFromObj*                           pCloneSrc_                  = nullptr; ///< クローン元のインスタンス

private: /// 非公開メンバ関数
    void _CreateVertexResource();
    void _LoadModelTexture();
    void _CopyFrom(ModelFromObj* pCopySrc);

private: /// 他クラスのインスタンス
    std::string filePath_;
    std::string directoryPath_;
};