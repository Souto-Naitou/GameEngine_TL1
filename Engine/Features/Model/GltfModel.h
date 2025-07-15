#pragma once

#include <Features/Model/IModel.h>
#include <Features/Model/ModelData.h>
#include <Features/Model/Animation.h>
#include <Features/Model/Skeleton.h>
#include <Features/Model/SkinCluster.h>
#include <wrl/client.h>
#include <Features/TimeMeasurer/TimeMeasurer.h>
#include "SkinCluster.h"

class GltfModel : public IModel 
{
public:
    // Common functions
    ~GltfModel()                                override;
    void Initialize()                           override;
    void Finalize()                             override;
    void Update()                               override;
    void Draw(ID3D12GraphicsCommandList* _cl)   override;
    void CreateGPUResource()                    override;
    void Clone(IModel* _src)                    override;

    // Getter
    D3D12_VERTEX_BUFFER_VIEW    GetVertexBufferView()   const   override;
    size_t                      GetVertexCount()        const   override;
    D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandle()   const   override;
    bool                        IsEndLoading()          const   override;
    ModelData*                  GetModelData()                  override;
    Animation*                  GetAnimationData();
    Skeleton*                   GetSkeleton();
    SkinCluster*                GetSkinCluster();

    // Setter
    void ChangeTexture(D3D12_GPU_DESCRIPTOR_HANDLE _texSrvHnd) override;

private:
    Microsoft::WRL::ComPtr<ID3D12Resource>  vertexResource_         = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource>  indexResource_          = nullptr;  //< インデックスバッファリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>  wellResource_           = nullptr;
    ModelData                               modelData_              = {};
    Skeleton                                skeleton_;                          //< スケルトンデータ
    Animation                               animationData_          = {};
    SkinCluster                             skinCluster_;                        //< スキンクラスター
    D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_       = {};
    D3D12_INDEX_BUFFER_VIEW                 indexBufferView_        = {};
    VertexData*                             vertexData_             = nullptr;  //< 頂点データのポインタ
    uint32_t*                               indexData_              = nullptr;  //< インデックスデータのポインタ
    D3D12_GPU_DESCRIPTOR_HANDLE             textureSrvHandleGPU_    = {};
    D3D12_GPU_DESCRIPTOR_HANDLE             wellSrvHandleGPU_       = {};
    uint32_t                                wellSRVIndex_           = 0u;
    bool                                    isReadyDraw_            = false;
    GltfModel*                              pCloneSrc_              = nullptr;  //< クローン元のインスタンス
    bool                                    isOverwroteTexture_     = false;    //< テクスチャを上書きしたかどうか
    TimeMeasurer                            timer_                  = {};       //< タイマー

    #ifdef _DEBUG
    bool    is_called_finalize_    = false;     //< Finalizeが呼ばれたかどうか
    #endif // _DEBUG

    void    _CreateVertexResource();
    void    _CreateIndexResource();
    void    _LoadModelTexture();
    void    _CopyFrom(GltfModel* _pCopySrc);
    void    _UpdateLocalMatrixByAnimation();
    void    _UpdateSkeleton();
    void    _UpdateSkinCluster();
    void    _ApplyAnimationToSkeleton();
    void    _CreateWellResource();
};