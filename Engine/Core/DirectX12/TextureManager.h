#pragma once

#include <string>
#include <Externals/DirectXTex/DirectXTex.h>
#include <wrl.h>
#include <d3d12.h>
#include <vector>
#include <Core/DirectX12/SRVManager.h>
#include <unordered_map>
#include <Utility/FilePathSearcher/FilePathSearcher.h>

class TextureManager
{
public:
    TextureManager(const TextureManager&) = delete;
    TextureManager(const TextureManager&&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&&) = delete;

    static TextureManager* GetInstance() { static TextureManager instance; return &instance;}

    void Initialize(SRVManager* _srvManager);

    /// <summary>
    /// テクスチャファイルの読み込み
    /// </summary>
    /// <param name="_filePath">テクスチャファイルのパス</param>
    void LoadTexture(const std::string& _filePath);


public: /// Setter
    void AddSearchPath(const std::string& _path) { filePathSearcher_.AddSearchPath(_path); }


public: /// Getter
    const DirectX::TexMetadata& GetMetaData(const std::string& _filePath);
    uint32_t GetSrvIndex(const std::string& _filePath);
    D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& _filePath);

private:
    struct TextureData
    {
        DirectX::TexMetadata metadata;
        Microsoft::WRL::ComPtr<ID3D12Resource> resource;
        uint32_t srvIndex;
        D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
        D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
    };

    std::unordered_map<std::string, TextureData> textureDataMap_;
    FilePathSearcher filePathSearcher_ = {};

private:
    SRVManager* srvManager_ = nullptr;

private:
    TextureManager() = default;
    ~TextureManager() = default;

};