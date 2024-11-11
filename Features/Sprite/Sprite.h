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
        Vector4 color;
        int32_t enableLighting;
        float padding[3];
        Matrix4x4 uvTransform;
    };

    struct TransformationMatrix
    {
        Matrix4x4 wvp;
        Matrix4x4 world;
    };

    Sprite();
    ~Sprite();

    void Initialize(SpriteSystem* _spriteSystem, std::string _filepath, Vector3 _scale, Vector3 _rotate, Vector3 _transform);
    void Update();
    void Draw();
    void Finalize();

private: /// 他クラスが所持するインスタンスへのポインタ
    SpriteSystem* pSpriteSystem_ = nullptr;
    DirectX12* pDx12_ = nullptr;
    ID3D12Device* device_ = nullptr;

private: /// メンバ変数

    Transform transform_;
    Transform uvTransform_;
    Matrix4x4 uvTransformMatrix_;

    /// バッファリソース
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;

    /// バッファリソースのデータ
    VertexData* vertexData_ = nullptr;
    uint32_t* indexData_ = nullptr;

    /// バッファリソースの使い方を示すビュー
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
    D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};

    /// マテリアル
    Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
    Material* materialData_ = nullptr;

    /// 変換行列
    Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;
    TransformationMatrix* transformationMatrixData_ = nullptr;

private: /// メンバ関数
    void CreateVertexResource();
    void CreateIndexResource();
    void CreateVertexBufferView();
    void CreateIndexBufferView();

    void MapVertexData();
    void MapIndexData();

    void CreateMaterialResource();
    void CreateTransformationMatrixResource();
};