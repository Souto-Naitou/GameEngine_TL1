#include "ConfigManager.h"

#include <Utility/JSONIO/JSONIO.h>

void ConfigManager::Initialize(const std::string& _cfgPath)
{
    LoadConfig(_cfgPath);
}

const cfg::ConfigData& ConfigManager::GetConfigData() const
{
    return configData_;
}

void ConfigManager::LoadConfig(const std::string& _cfgPath)
{
    auto& j = JSONIO::GetInstance()->Load(_cfgPath);
    configData_ = j;
}
