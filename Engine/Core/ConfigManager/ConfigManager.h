#pragma once

#include "./ConfigType.h"
#include <ClassStyles/SingletonStyle.h>
#include <string>

class ConfigManager : public SingletonStyle<ConfigManager>
{
    friend class SingletonStyle<ConfigManager>;
public:
    // Common functions
    void Initialize(const std::string& _cfgPath);
    
    // Getters
    const cfg::ConfigData& GetConfigData() const;

private:
    // Internal functions
    void LoadConfig(const std::string& _cfgPath);

    // Config data
    cfg::ConfigData configData_ = {};
};