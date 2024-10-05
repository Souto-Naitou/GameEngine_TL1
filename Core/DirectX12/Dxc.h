#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <dxcapi.h>
#include <string>

#include <BaseConfiguratorForComPtr.h>

namespace DxcUtils
{
    class Configurator : public BaseConfiguratorForComPtr<IDxcUtils>
    {
    public:
        Configurator(const Configurator&) = delete;
        Configurator(const Configurator&&) = delete;
        Configurator& operator=(const Configurator&) = delete;
        Configurator& operator=(const Configurator&&) = delete;

        void Initialize() override;
        static Configurator* GetInstance() { static Configurator instance; return &instance; }

    private:
        Configurator() = default;
    };
}

namespace DxcCompiler
{
    class Configurator : public BaseConfiguratorForComPtr<IDxcCompiler3>
    {
    public:
        Configurator(const Configurator&) = delete;
        Configurator(const Configurator&&) = delete;
        Configurator& operator=(const Configurator&) = delete;
        Configurator& operator=(const Configurator&&) = delete;

        void Initialize() override;
        static Configurator* GetInstance() { static Configurator instance; return &instance; }

    private:
        Configurator() = default;
    };
}

namespace IncludeHandler
{
    class Configurator : public BaseConfiguratorForComPtr<IDxcIncludeHandler>
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
    class Configurator : public BaseConfiguratorForComPtr<IDxcBlob>
    {
    public:
        void Initialize();
    };
}

namespace PixelShaderBlob
{
    class Configurator : public BaseConfiguratorForComPtr<IDxcBlob>
    {
    public:
        void Initialize() override;
    };
}

Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
    const std::wstring& filePath,
    const wchar_t* profile
);