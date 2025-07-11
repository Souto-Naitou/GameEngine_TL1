#pragma once

#include <Features/Model/IModel.h>
#include <Features/Model/ModelData.h>
#include <Features/Model/Animation.h>
#include <Features/Model/Skeleton.h>
#include <wrl/client.h>
#include <Features/TimeMeasurer/TimeMeasurer.h>

class GltfModel : public IModel 
{
public:
    // Common functions
    ~GltfModel()                                override = default;
    void Initialize()                           override;
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

    // Setter
    void ChangeTexture(D3D12_GPU_DESCRIPTOR_HANDLE _texSrvHnd) override;

private:
    Microsoft::WRL::ComPtr<ID3D12Resource>  vertexResource_         = nullptr;
    ModelData                               modelData_              = {};
    Skeleton                                skeleton_;                          //< スケルトンデータ
    Animation                               animationData_          = {};
    D3D12_VERTEX_BUFFER_VIEW                vertexBufferView_       = {};
    VertexData*                             vertexData_             = nullptr;  //< 頂点データのポインタ
    D3D12_GPU_DESCRIPTOR_HANDLE             textureSrvHandleGPU_    = {};
    bool                                    isReadyDraw_            = false;
    GltfModel*                              pCloneSrc_              = nullptr;  //< クローン元のインスタンス
    bool                                    isOverwroteTexture_     = false;    //< テクスチャを上書きしたかどうか
    TimeMeasurer                            timer_                  = {};       //< タイマー

    void _CreateVertexResource();
    void _LoadModelTexture();
    void _CopyFrom(GltfModel* _pCopySrc);
    void _UpdateLocalMatrixByAnimation();
    void _UpdateSkeleton();
    void _ApplyAnimationToSkeleton();
};