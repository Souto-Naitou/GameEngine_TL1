#include "TextureManager.h"
#include "DirectX12.h"
#include <Core/DirectX12/Helper/DX12Helper.h>
#include <Utility/ConvertString/ConvertString.h>



void TextureManager::Initialize(SRVManager* _srvManager)
{
    srvManager_ = _srvManager;
    textureDataMap_.reserve(DirectX12::kMaxSRVCount_);
}

void TextureManager::LoadTexture(const std::string& _filePath)
{
    /// すでに読み込まれている場合は読み込まない
    if (textureDataMap_.contains(_filePath))
    {
        return;
    }

    assert(!srvManager_->IsFull() && "SRVがいっぱいです");

    DirectX12* pDx12 = DirectX12::GetInstance();
    ID3D12Device* device = pDx12->GetDevice();

    TextureData& textureData = textureDataMap_[_filePath];

    DirectX::ScratchImage image{};
    std::wstring filePathW = ConvertString(_filePath);
    HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
    assert(SUCCEEDED(hr));


    DirectX::TexMetadata metadata = image.GetMetadata();
    if (!metadata.width == 1 || !metadata.height == 1)
    {
        hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, image);
        metadata = image.GetMetadata();
        assert(SUCCEEDED(hr));
    }

    textureData.metadata = metadata;
    textureData.resource = DX12Helper::CreateTextureResource(DirectX12::GetInstance()->GetDevice(), textureData.metadata);
    DX12Helper::UploadTextureData(textureData.resource, image);

    uint32_t kDescriptorSizeSRV = DirectX12::GetInstance()->GetDescriptorSizeSRV();
    textureData.srvIndex = srvManager_->Allocate();
    textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
    textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

    srvManager_->CreateForTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData.metadata.format, static_cast<UINT>(textureData.metadata.mipLevels));
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& _filePath)
{
    TextureData& textureData = textureDataMap_[_filePath];
    return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& _filePath)
{
    TextureData& textureData = textureDataMap_[_filePath];
    return textureData.metadata;
}

uint32_t TextureManager::GetSrvIndex(const std::string& _filePath)
{
    TextureData& textureData = textureDataMap_[_filePath];
    return textureData.srvIndex;
}
