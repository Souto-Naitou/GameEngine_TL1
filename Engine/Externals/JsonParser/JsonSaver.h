#pragma once

#include <filesystem>
#include "jsontypes.h"

class JsonSaver
{
public:
    JsonSaver(const JsonSaver&) = delete;
    JsonSaver& operator=(const JsonSaver&) = delete;
    JsonSaver(const JsonSaver&&) = delete;
    JsonSaver& operator=(const JsonSaver&&) = delete;

    static JsonSaver* GetInstance()
    {
        static JsonSaver instance;
        return &instance;
    }

    /// <summary>
    /// JSONファイルを保存する
    /// </summary>
    /// <param name="_path">保存先ファイルパス</param>
    /// <param name="_value">JSONデータ</param>
    void Run(std::filesystem::path _path, JsonValue _value);


private:
    JsonSaver() = default;
    ~JsonSaver() = default;

    std::string GetString(const JsonValue& _value, int _indent, bool _isSameLine);

    uint32_t indentSize_ = 4u;
};