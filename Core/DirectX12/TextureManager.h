#pragma once

#include <string>
#include <externals/DirectXTex/DirectXTex.h>
#include <wrl.h>
#include <d3d12.h>
#include <vector>

class TextureManager
{
public:
    TextureManager(const TextureManager&) = delete;
    TextureManager(const TextureManager&&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&&) = delete;

    static TextureManager* GetInstance() { static TextureManager instance; return &instance;}

    void Initialize();

    /// <summary>
    /// テクスチャファイルの読み込み
    /// </summary>
    /// <param name="_filePath">テクスチャファイルのパス</param>
    void LoadTexture(const std::string& _filePath);

    uint32_t GetTextureIndexByFilePath(const std::string& _filePath);
    D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t _textureIndex);

private:
    struct TextureData
    {
        std::string filePath;
        DirectX::TexMetadata metadata;
        Microsoft::WRL::ComPtr<ID3D12Resource> resource;
        D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
        D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
    };

    static uint32_t kSRVIndexTop;

    std::vector<TextureData> textureDataContainer_;

private:
    TextureManager() = default;
    ~TextureManager() = default;

};