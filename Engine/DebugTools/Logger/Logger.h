#pragma once


#include <string>
#include <JsonParser/JsonLoader.h>
#include <list>


/// <summary>
/// ログ出力クラス
/// </summary>
class Logger
{
private:
    struct LogData
    {
        std::string date;
        std::string time;
        std::string status;
        std::string className;
        std::string action;
        std::string message;
    };


public:
    Logger(Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    static Logger* GetInstance()
    {
        static Logger instance;
        return &instance;
    }

    void Initialize();
    void Save();


public:
    void LogError(
        const std::string& _className,
        const std::string& _action,
        const std::string& _message
    );
    
    void LogWarning(
        const std::string& _className,
        const std::string& _action,
        const std::string& _message
    );

    void LogInfo(
        const std::string& _className,
        const std::string& _action,
        const std::string& _message
    );

    void LogForOutput(const std::string& _message);


private:
    void Log(
        const std::string& _status,
        const std::string& _className,
        const std::string& _action,
        const std::string& _message
    );
    void LogJson(
        const std::string& _date,
        const std::string& _time,
        const std::string& _status,
        const std::string& _className,
        const std::string& _action,
        const std::string& _message
    );


private:
    Json::Value jsonValue_ = {};
    std::list<LogData> logData_ = {};
    const std::string folderPath_ = "Log/";
    std::string fileName_ = "";


private:
    JsonLoader* pJsonLoader_ = nullptr;


private:
    Logger() = default;
    ~Logger() = default;
};