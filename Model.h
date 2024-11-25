#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <ModelSystem.h>
#include <string>

struct ModelData;
struct Material;

class Model
{
public: /// 公開メンバ関数
    void Initialize(const std::string& _filePath);
    void Update();
    void Draw();


private: /// メンバ変数
    static const std::string kDefaultDirectoryPath;

    ModelSystem* pModelCommon_;
    ModelData modelData_;
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
    VertexData* vertexData_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
    Material* materialData_ = nullptr;


private: /// 非公開メンバ関数
    void CreateMaterialResource();
    void CreateVertexResource();


private: /// 他クラスのインスタンス
    ID3D12Device* device_ = nullptr;
};