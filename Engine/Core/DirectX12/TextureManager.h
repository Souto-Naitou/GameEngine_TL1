#pragma once

#include <string>
#include <Externals/DirectXTex/DirectXTex.h>
#include <wrl.h>
#include <d3d12.h>
#include <Core/DirectX12/SRVManager.h>
#include <unordered_map>
#include <Utility/PathResolver/PathResolver.h>
#include <Core/DirectX12/TextureResource/TextureResource.h>

class TextureManager : public EngineFeature
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
    void AddSearchPath(const std::string& _path) { pathResolver_.AddSearchPath(_path); }


public: /// Getter
    [[nodiscard]] const DirectX::TexMetadata&   GetMetaData(const std::string& _filePath);
    [[nodiscard]] uint32_t                      GetSrvIndex(const std::string& _filePath);
    [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE   GetSrvHandleGPU(const std::string& _filePath);
    [[nodiscard]] const TextureResource&        GetTextureResource(const std::string& _filePath);

private:
    struct TextureData
    {
        DirectX::TexMetadata metadata = {};
        TextureResource textureResource = {};
    };

    std::unordered_map<std::string, TextureData> textureDataMap_;
    PathResolver pathResolver_ = {};

private:
    SRVManager* srvManager_ = nullptr;

private:
    TextureManager() = default;
    ~TextureManager() = default;

};