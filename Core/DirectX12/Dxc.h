#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <dxcapi.h>
#include <string>

#include <BaseConfigurator.h>

namespace DxcUtils
{
    class Configurator : public BaseConfigurator<Microsoft::WRL::ComPtr<IDxcUtils>>
    {
    public:
        Configurator(const Configurator&) = delete;
        Configurator(const Configurator&&) = delete;
        Configurator& operator=(const Configurator&) = delete;
        Configurator& operator=(const Configurator&&) = delete;

        void Initialize();
        static Configurator* GetInstance() { static Configurator instance; return &instance; }
    private:
        Configurator() = default;
    };
}

namespace DxcCompiler
{
    class Configurator : public BaseConfigurator<Microsoft::WRL::ComPtr<IDxcCompiler3>>
    {
    public:
        Configurator(const Configurator&) = delete;
        Configurator(const Configurator&&) = delete;
        Configurator& operator=(const Configurator&) = delete;
        Configurator& operator=(const Configurator&&) = delete;

        void Initialize();
        static Configurator* GetInstance() { static Configurator instance; return &instance; }

    private:
        Configurator() = default;
    };
}

namespace IncludeHandler
{
    class Configurator : public BaseConfigurator<Microsoft::WRL::ComPtr<IDxcIncludeHandler>>
    {
    public:
        Configurator(const Configurator&) = delete;
        Configurator(const Configurator&&) = delete;
        Configurator& operator=(const Configurator&) = delete;
        Configurator& operator=(const Configurator&&) = delete;

        void Initialize();
        static Configurator* GetInstance() { static Configurator instance; return &instance; }

    private:
        Configurator() = default;
    };
}

namespace VertexShaderBlob
{
    class Configurator : public BaseConfigurator<Microsoft::WRL::ComPtr<IDxcBlob>>
    {
    public:
        void Initialize();

    private:
        DxcUtils::Configurator* pDxcUtils_;
        DxcCompiler::Configurator* pDxcCompiler_;
        IncludeHandler::Configurator* pIncludeHandler_;
    };
}

namespace PixelShaderBlob
{
    class Configurator : public BaseConfigurator<Microsoft::WRL::ComPtr<IDxcBlob>>
    {
    public:
        void initialize();

    private:
        DxcUtils::Configurator* pDxcUtils_;
        DxcCompiler::Configurator* pDxcCompiler_;
        IncludeHandler::Configurator* pIncludeHandler_;
    };
}

Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
    const std::wstring& filePath,
    const wchar_t* profile,
    const Microsoft::WRL::ComPtr<IDxcUtils>& dxcUtils,
    const Microsoft::WRL::ComPtr<IDxcCompiler3>& dxcCompiler,
    const Microsoft::WRL::ComPtr<IDxcIncludeHandler>& includeHandler
);