#include "TextureManager.h"
#include "DirectX12.h"
#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Utility/ConvertString/ConvertString.h>
#include <Core/ConfigManager/ConfigManager.h>


void TextureManager::Initialize(SRVManager* _srvManager)
{
    srvManager_ = _srvManager;
    textureDataMap_.reserve(DirectX12::kMaxSRVCount_);

    auto& cfgData = ConfigManager::GetInstance()->GetConfigData();
    for (auto& path : cfgData.texture_paths)
    {
        this->AddSearchPath(path);
    }
}

void TextureManager::LoadTexture(const std::string& _filePath)
{
    std::string fullPath = filePathSearcher_.GetFilePath(_filePath);

    /// すでに読み込まれている場合は読み込まない
    if (textureDataMap_.contains(fullPath))
    {
        return;
    }

    assert(!srvManager_->IsFull() && "SRVがいっぱいです");

    TextureData& textureData = textureDataMap_[fullPath];

    DirectX::ScratchImage image{};
    std::wstring filePathW = ConvertString(fullPath);
    HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
    assert(SUCCEEDED(hr));

    DirectX::TexMetadata metadata = image.GetMetadata();

    if (DirectX::IsCompressed(metadata.format)) {
        DirectX::ScratchImage decompressed;
        DirectX::Decompress(image.GetImages(), image.GetImageCount(), metadata, DXGI_FORMAT_R8G8B8A8_UNORM, decompressed);
        image = std::move(decompressed);
    }

    if (metadata.width > 32 && metadata.height > 32)
    {
        DirectX::ScratchImage mipChain = {};
        hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipChain);
        image = std::move(mipChain);
        metadata = image.GetMetadata();
        assert(SUCCEEDED(hr));
    }

    textureData.metadata = metadata;
    textureData.textureResource.SetResource(DX12Helper::CreateTextureResource(pDx12_->GetDevice(), textureData.metadata));
    DX12Helper::UploadTextureData(textureData.textureResource.GetResource(), image);

    uint32_t srvIndex = srvManager_->Allocate();
    auto srvHandleCPU = srvManager_->GetCPUDescriptorHandle(srvIndex);
    auto srvHandleGPU = srvManager_->GetGPUDescriptorHandle(srvIndex);

    textureData.textureResource.SetSRV(
        srvIndex,
        srvHandleCPU,
        srvHandleGPU
    );

    srvManager_->CreateForTexture2D(srvIndex, textureData.textureResource.GetResource(), textureData.metadata.format, static_cast<UINT>(textureData.metadata.mipLevels));
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& _filePath)
{
    std::string fullPath = filePathSearcher_.GetFilePath(_filePath);
    const TextureData& textureData = textureDataMap_[fullPath];
    return textureData.textureResource.GetSRVHandleGPU();
}

const TextureResource& TextureManager::GetTextureResource(const std::string& _filePath)
{
    std::string fullPath = filePathSearcher_.GetFilePath(_filePath);
    const TextureData& textureData = textureDataMap_[fullPath];
    return textureData.textureResource;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& _filePath)
{
    std::string fullPath = filePathSearcher_.GetFilePath(_filePath);
    const TextureData& textureData = textureDataMap_[fullPath];
    return textureData.metadata;
}

uint32_t TextureManager::GetSrvIndex(const std::string& _filePath)
{
    std::string fullPath = filePathSearcher_.GetFilePath(_filePath);
    const TextureData& textureData = textureDataMap_[fullPath];
    return textureData.textureResource.GetSRVIndex();
}
