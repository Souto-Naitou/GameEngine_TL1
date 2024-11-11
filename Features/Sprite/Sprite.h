#pragma once

#include <Vector4.h>
#include <Vector3.h>
#include <Vector2.h>
#include "DirectX12.h"
#include "structs.h"

class SpriteSystem;


struct VertexData
{
    Vector4 position;
    Vector2 texcoord;
    Vector3 normal;
};


class Sprite
{
public:
    struct Material
    {
        Vector4     color;
        int32_t     enableLighting;
        float       padding[3];
        Matrix4x4   uvTransform;
    };

    struct TransformationMatrix
    {
        Matrix4x4 wvp;
        Matrix4x4 world;
    };

    Sprite();
    ~Sprite();

    void                Initialize(SpriteSystem* _spriteSystem, std::string _filepath);
    void                Update();
    void                Draw();
    void                Finalize();


public: /// Getter
    const float         GetRotation()       const               { return rotate_; }
    const Vector2&      GetPosition()       const               { return translate_; }
    const Vector4&      GetColor()          const               { return materialData_->color; }
    const Vector2&      GetSize()           const               { return size_; }
    const Vector2&      GetAnchorPoint()    const               { return anchorPoint_; }
    const bool          GetFlipX()          const               { return isFlipX; }
    const bool          GetFlipY()          const               { return isFlipY; }
    const Vector2&      GetTextureLeftTop() const               { return textureLeftTop_; }
    const Vector2&      GetTextureSize()    const               { return textureSize_; }

public: /// Setter
    void                SetRotation(const float _rotation)      { rotate_ = _rotation; }
    void                SetPosition(const Vector2& _position)   { translate_ = _position; }
    void                SetColor(const Vector4& _color)         { materialData_->color = _color; }
    void                SetSize(const Vector2& _size)           { size_ = _size; }
    void                SetAnchorPoint(const Vector2& _anchor)  { anchorPoint_ = _anchor; }
    void                SetFlipX(const bool _isFlipX)           { isFlipX = _isFlipX; }
    void                SetFlipY(const bool _isFlipY)           { isFlipY = _isFlipY; }
    void                SetTextureLeftTop(const Vector2& _lt)   { textureLeftTop_ = _lt; }
    void                SetTextureSize(const Vector2& _size)    { textureSize_ = _size; }


private: /// 他クラスが所持するインスタンスへのポインタ
    SpriteSystem*                               pSpriteSystem_                  = nullptr;
    DirectX12*                                  pDx12_                          = nullptr;
    ID3D12Device*                               device_                         = nullptr;


private: /// メンバ変数

    Transform                                   transform_                      = {};       // 位置、回転、拡大縮小

    Vector2                                     size_                           = {};       // サイズ
    float                                       rotate_                         = {};       // 回転角
    Vector2                                     translate_                      = {};       // 位置
    Vector2                                     anchorPoint_                    = {};       // アンカーポイント

    bool                                        isFlipX                         = false;    // X軸反転
    bool                                        isFlipY                         = false;    // Y軸反転

    Vector2                                     textureLeftTop_                 = {};       // テクスチャの左上uv
    Vector2                                     textureSize_                    = {};       // テクスチャのuv範囲

    Transform                                   uvTransform_                    = {};       // UV変換
    Matrix4x4                                   uvTransformMatrix_              = {};       // UV変換行列

    uint32_t                                    textureIndex_                   = 0;        // テクスチャハンドル

    /// バッファリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>      vertexResource_                 = nullptr;  // 頂点リソース
    Microsoft::WRL::ComPtr<ID3D12Resource>      indexResource_                  = nullptr;  // インデックスリソース

    /// バッファリソースのデータ
    VertexData*                                 vertexData_                     = nullptr;  // 頂点データ
    uint32_t*                                   indexData_                      = nullptr;  // インデックスデータ

    /// バッファリソースの使い方を示すビュー
    D3D12_VERTEX_BUFFER_VIEW                    vertexBufferView_               = {};       // 頂点バッファビュー
    D3D12_INDEX_BUFFER_VIEW                     indexBufferView_                = {};       // インデックスバッファービュー

    /// マテリアル
    Microsoft::WRL::ComPtr<ID3D12Resource>      materialResource_               = nullptr;  // マテリアルリソース
    Material*                                   materialData_                   = nullptr;  // マテリアルデータ

    /// 変換行列
    Microsoft::WRL::ComPtr<ID3D12Resource>      transformationMatrixResource_   = nullptr;  // 変換行列リソース
    TransformationMatrix*                       transformationMatrixData_       = nullptr;  // 変換行列データ


private: /// メンバ関数
    void CreateVertexResource();
    void CreateIndexResource();
    void CreateVertexBufferView();
    void CreateIndexBufferView();

    void MapVertexData();
    void MapIndexData();

    void CreateMaterialResource();
    void CreateTransformationMatrixResource();
    void AdjustSpriteSize();
};