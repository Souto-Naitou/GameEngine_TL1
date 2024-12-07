#pragma once

#include "LineSystem.h"
#include <d3d12.h>
#include <Common/structs.h>
#include <wrl.h>
#include <Core/DirectX12/DirectX12.h>

class Line
{
public:
    Line() = default;
    ~Line() = default;

    void Initialize();
    void Finalize();
    void Update();
    void Draw();


public: /// Getter
    Vector3* GetVertices() { return vertices_; }
    Vector4& GetColorData() { return color_; }


private:
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
    Vector3* pVertexData_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> wvpMatrixResource_;
    Matrix4x4* pWVPMatrixData_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> colorResource_;
    Vector4* pColorData_ = nullptr;

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

    Vector3 vertices_[2] = {};
    Vector4 color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

private:
    void CreateVertexResource();
    void CreateWVPMatrixResource();
    void CreateColorResource();

private:
    LineSystem* pLineSystem_ = nullptr;
    DirectX12* pDx12_ = nullptr;
    ID3D12Device* device_ = nullptr;
    GameEye* pGameEye_ = nullptr;
};