#include "Logger.h"
#include <chrono>
#include <format>
#include <fstream>

#include <debugapi.h>

void Log(const std::string& _message)
{
	OutputDebugStringA(_message.c_str());
	return;
}

void Logger::Initialize()
{
    pJsonLoader_ = &JsonLoader::GetInstance();

    /// ルートを作成
    jsonValue_.type = Json::Value::Type::Object;
    jsonValue_.value = Json::Object();

    /// Logs配列を作成
    Json::Array logs = {};
    std::get<Json::Object>(jsonValue_.value)["Logs"] = Json::NewValue(logs);


    /// ファイル名を設定
    auto now = std::chrono::system_clock::now();
    auto now_sec = std::chrono::floor<std::chrono::seconds>(now);
    fileName_ = std::format("{:%Y%m%d_%H%M%S}", now_sec);

    return;
}

void Logger::Save()
{
    auto now = std::chrono::system_clock::now();

    std::string filepath = folderPath_ + fileName_;
    pJsonLoader_->SaveFile(filepath + ".json", jsonValue_);

    std::string filePayload;
    for (auto& log : logData_)
    {
        filePayload += "[" + log.status + "] ";
        filePayload += log.date + " ";
        filePayload += log.time + ", ";
        filePayload += log.className + ", ";
        filePayload += log.action + ", ";
        filePayload += log.message + "\n";
    }

    /// 改行を削除
    filePayload.erase(filePayload.end() - 1);

    std::ofstream ofs(filepath + ".log");
    ofs << filePayload;
    ofs.close();

    return;
}

void Logger::LogError(const std::string& _className, const std::string& _action, const std::string& _message)
{
    Log("Error", _className, _action, _message);
}

void Logger::LogWarning(const std::string& _className, const std::string& _action, const std::string& _message)
{
    Log("Warning", _className, _action, _message);
}

void Logger::LogInfo(const std::string& _className, const std::string& _action, const std::string& _message)
{
    Log("Info", _className, _action, _message);
}

void Logger::LogForOutput(const std::string& _message)
{
    OutputDebugStringA(_message.c_str());
}

void Logger::Log(const std::string& _status, const std::string& _className, const std::string& _action, const std::string& _message)
{
    auto now = std::chrono::system_clock::now();
    auto now_sec = std::chrono::floor<std::chrono::seconds>(now);


    std::string date = std::format("{:%Y/%m/%d}", now_sec);
    std::string time = std::format("{:%H:%M:%S}", now_sec);

    LogData data = {};
    data.date = date;
    data.time = time;
    data.status = _status;
    data.className = _className;
    data.action = _action;
    data.message = _message;


    logData_.emplace_back(data);
    LogJson(date, time, _status, _className, _action, _message);

    OutputDebugStringA(std::format("[{}] {} {}, {}, {}, {}\n", _status, date, time, _className, _action, _message).c_str());

    this->Save();
}

void Logger::LogJson(const std::string& _date, const std::string& _time, const std::string& _status, const std::string& _className, const std::string& _action, const std::string& _message)
{
    Json::Array logs = jsonValue_["Logs"];
    Json::Object data = {};
    data["Date"] = Json::NewValue(_date);
    data["Time"] = Json::NewValue(_time);
    data["Status"] = Json::NewValue(_status);
    data["ClassName"] = Json::NewValue(_className);
    data["Action"] = Json::NewValue(_action);
    data["Message"] = Json::NewValue(_message);

    logs.push_back(Json::NewValue(data));
    jsonValue_["Logs"] = Json::NewValue(logs);

    return;
}
