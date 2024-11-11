#include "TextureManager.h"
#include "DirectX12.h"
#include "DX12Helper.h"

#include <ConvertString.h>

uint32_t TextureManager::kSRVIndexTop = 1ui32;

void TextureManager::Initialize()
{
    textureDataContainer_.reserve(DirectX12::kMaxSRVCount_);
}

void TextureManager::LoadTexture(const std::string& _filePath)
{
    auto itr = std::find_if(
        textureDataContainer_.begin(),
        textureDataContainer_.end(),
        [&_filePath](const TextureData& _textureData)
    { return _textureData.filePath == _filePath; });

    if (itr != textureDataContainer_.end()) { return; }

    assert(textureDataContainer_.size() + kSRVIndexTop < DirectX12::kMaxSRVCount_);

    DirectX12* pDx12 = DirectX12::GetInstance();
    ID3D12Device* device = pDx12->GetDevice();

    DirectX::ScratchImage image{};
    std::wstring filePathW = ConvertString(_filePath);
    HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
    assert(SUCCEEDED(hr));

    DirectX::ScratchImage mipImages{};
    hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
    assert(SUCCEEDED(hr));


    // テクスチャデータを追加
    textureDataContainer_.resize(textureDataContainer_.size() + 1);
    // 追加したテクスチャデータの参照を取得
    TextureData& textureData = textureDataContainer_.back();

    textureData.filePath = _filePath;
    textureData.metadata = mipImages.GetMetadata();
    textureData.resource = DX12Helper::CreateTextureResource(DirectX12::GetInstance()->GetDevice(), textureData.metadata);
    DX12Helper::UploadTextureData(textureData.resource, mipImages);

    uint32_t srvIndex = static_cast<uint32_t>(textureDataContainer_.size() - 1) + kSRVIndexTop;
    ID3D12DescriptorHeap* srvDescriptorHeap = DirectX12::GetInstance()->GetSRVDescriptorHeap();
    uint32_t kDescriptorSizeSRV = DirectX12::GetInstance()->GetDescriptorSizeSRV();
    textureData.srvHandleCPU = DX12Helper::GetCPUDescriptorHandle(srvDescriptorHeap,kDescriptorSizeSRV , srvIndex);
    textureData.srvHandleGPU = DX12Helper::GetGPUDescriptorHandle(srvDescriptorHeap, kDescriptorSizeSRV, srvIndex);

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureData.metadata.format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Texture2D.MipLevels = textureData.metadata.mipLevels;

    device->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& _filePath)
{
    auto itr = std::find_if(
        textureDataContainer_.begin(),
        textureDataContainer_.end(),
        [&_filePath](const TextureData& _textureData)
    { return _textureData.filePath == _filePath; });

    if (itr != textureDataContainer_.end())
    {
        uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDataContainer_.begin(), itr));
        return textureIndex;
    }

    // 検索で見つからなかった場合は止める
    assert(false);
    return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(uint32_t _textureIndex)
{
    assert(_textureIndex < textureDataContainer_.size());

    TextureData& textureData = textureDataContainer_[_textureIndex];
    return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(uint32_t _textureIndex)
{
    // 範囲外チェック
    assert(_textureIndex < textureDataContainer_.size());

    TextureData& textureData = textureDataContainer_[_textureIndex];
    return textureData.metadata;
}
