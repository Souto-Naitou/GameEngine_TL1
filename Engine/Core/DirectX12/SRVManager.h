#pragma once

#include <d3d12.h>
#include <cstdint>

class SRVManager
{
public:
    SRVManager(const SRVManager&) = delete;
    SRVManager(const SRVManager&&) = delete;
    SRVManager& operator=(const SRVManager&) = delete;
    SRVManager& operator=(const SRVManager&&) = delete;
    static SRVManager* GetInstance()
    {
        static SRVManager instance;
        return &instance;
    }
    void Initialize();

private:
    SRVManager() = default;
    ~SRVManager() = default;
};