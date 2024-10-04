#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <dxcapi.h>
#include <string>

namespace ShaderBin
{


    class Configurator
    {
    public:
        void Initialize();
        inline IDxcBlob* GetVertexShaderBlob() { return vertexShaderBlob_.Get(); }
        inline IDxcBlob* GetPixelShaderBlob() { return pixelShaderBlob_.Get(); }

    private:
        Microsoft::WRL::ComPtr<IDxcBlob>            vertexShaderBlob_   = nullptr;
        Microsoft::WRL::ComPtr<IDxcBlob>            pixelShaderBlob_    = nullptr;
        Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
            const std::wstring& filePath,
            const wchar_t* profile,
            const Microsoft::WRL::ComPtr<IDxcUtils>& dxcUtils,
            const Microsoft::WRL::ComPtr<IDxcCompiler3>& dxcCompiler,
            const Microsoft::WRL::ComPtr<IDxcIncludeHandler>& includeHandler
        );
        Microsoft::WRL::ComPtr<IDxcUtils>           dxcUtils_           = nullptr;      // DirectX Shader Utilities
        Microsoft::WRL::ComPtr<IDxcCompiler3>       dxcCompiler_        = nullptr;      // DirectX Shader Compiler
        Microsoft::WRL::ComPtr<IDxcIncludeHandler>  includeHandler_     = nullptr;      // インクルードハンドラ
    };

}