#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <stdint.h>
#include <dxcapi.h>
#include <string>
#include <Externals/DirectXTex/DirectXTex.h>
#include <vector>

namespace DX12Helper
{
    /// <summary>
    /// デバイスを生成する
    /// </summary>
    /// <param name="_device">生成先</param>
    /// <param name="_adapter">使用するアダプタ</param>
    void CreateDevice(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<IDXGIAdapter4>& _adapter);


    /// <summary>
    /// エラー時に停止する
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_infoQ">情報キュー</param>
    void PauseError(Microsoft::WRL::ComPtr<ID3D12Device>& _device, Microsoft::WRL::ComPtr<ID3D12InfoQueue>& _infoQ);


    /// <summary>
    /// ディスクリプタヒープの生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_heapType">ヒープタイプ</param>
    /// <param name="_numDescriptors">ディスクリプタの数</param>
    /// <param name="_shaderVisible">シェーダーが参照可能か</param>
    /// <returns></returns>
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
        const Microsoft::WRL::ComPtr<ID3D12Device>& _device,
        D3D12_DESCRIPTOR_HEAP_TYPE _heapType,
        UINT _numDescriptors,
        bool _shaderVisible);


    /// <summary>
    /// DepthStencilTextureリソースを生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_width">幅</param>
    /// <param name="_height">高さ</param>
    /// <returns>リソース</returns>
    Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, int32_t _width, int32_t _height);


    /// <summary>
    /// シェーダーをコンパイル
    /// </summary>
    /// <param name="filePath">ファイルパス</param>
    /// <param name="profile">プロファイル</param>
    /// <param name="dxcUtils">DXCユーティリティ</param>
    /// <param name="dxcCompiler">DXCコンパイラ</param>
    /// <param name="includeHandler">インクルードハンドラ</param>
    /// <returns>コンパイル結果</returns>
    Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
        const std::wstring& filePath,
        const wchar_t* profile,
        const Microsoft::WRL::ComPtr<IDxcUtils>& dxcUtils,
        const Microsoft::WRL::ComPtr<IDxcCompiler3>& dxcCompiler,
        const Microsoft::WRL::ComPtr<IDxcIncludeHandler>& includeHandler);


    /// <summary>
    /// バッファリソースを生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_sizeInBytes">バイト数</param>
    /// <returns>リソース</returns>
    Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, size_t _sizeInBytes);


    /// <summary>
    /// テクスチャリソースを生成
    /// </summary>
    /// <param name="_filePath">ファイルパス</param>
    DirectX::ScratchImage LoadTexture(const std::string _filePath);

    Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const Microsoft::WRL::ComPtr<ID3D12Device>& _device, const DirectX::TexMetadata& _metadata);
    void UploadTextureData(const Microsoft::WRL::ComPtr<ID3D12Resource>& _texture, const DirectX::ScratchImage& _mipImages);
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& _descriptorHeap, uint32_t _descriptorSize, uint32_t _index);
    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& _descriptorHeap, uint32_t _descriptorSize, uint32_t _index);


    /// <summary>
    /// テクスチャリソースを生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    /// <param name="_srvDescriptorHeap">SRVディスクリプタヒープ</param>
    /// <param name="_kDescriptorSizeSRV">SRVディスクリプタサイズ</param>
    /// <param name="_path">ファイルパス</param>
    /// <param name="_textureResources">テクスチャリソース</param>
    /// <returns></returns>
    void CreateNewTexture(const Microsoft::WRL::ComPtr<ID3D12Device>& _device,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& _srvDescriptorHeap,
        const uint32_t _kDescriptorSizeSRV,
        const char* _path,
        std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& _textureResources);

    /// <summary>
    /// 頂点リソースを生成
    /// </summary>
    /// <param name="_device">デバイス</param>
    Microsoft::WRL::ComPtr<ID3D12Resource> CreateVertexResource(const Microsoft::WRL::ComPtr<ID3D12Device> _device, unsigned int _countVertex);
}