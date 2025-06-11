#include "ConfigManager.h"

#include <Utility/JSONIO/JSONIO.h>
#include <DebugTools/Logger/Logger.h>

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
    try
    {
        configData_ = JSONIO::GetInstance()->Load(_cfgPath);
    }
    catch (std::runtime_error& _rter)
    {
        Logger::GetInstance()->LogError("ConfigManager", __func__, _rter.what());
        configData_.window_title = "Nima Engine";
        JSONIO::GetInstance()->Save(_cfgPath, configData_);
    }
}
