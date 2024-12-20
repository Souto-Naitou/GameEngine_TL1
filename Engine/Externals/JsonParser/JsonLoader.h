#pragma once

#include <string>
#include <map>
#include <vector>
#include <variant>
#include <memory>
#include <filesystem>

#include "json.h"

class JsonLoader
{
public:
    JsonLoader(const JsonLoader&) = delete;
    JsonLoader& operator=(const JsonLoader&) = delete;
    JsonLoader(const JsonLoader&&) = delete;
    JsonLoader& operator=(const JsonLoader&&) = delete;

    static JsonLoader& GetInstance()
    {
        static JsonLoader instance;
        return instance;
    }


    /// <summary>
    /// JSONファイルを読み込む
    /// </summary>
    /// <param name="filename">ファイル名</param>
    void LoadFile(const std::string& filename);

    /// <summary>
    /// JSONファイルを再読み込む
    /// </summary>
    /// <param name="filename">ファイル名</param>
    void ReloadFile(const std::string& filename)
    {
        data_.erase(filename);
        LoadFile(filename);
    }

    /// <summary>
    /// JSONファイルを保存する
    /// </summary>
    /// <param name="filename">保存先ファイル名</param>
    /// <param name="value">JSONデータ</param>
    void SaveFile(const std::string& filename, const Json::Value& value);

    /// <summary>
    /// JSONファイルが存在するか確認する
    /// </summary>
    /// <param name="_filename">ファイル名</param>
    bool IsExist(const std::string& _filename);

public: /// 演算子オーバーロード
    const Json::Value& operator[](const std::string& _filename) const
    {
        return data_.at(_filename);
    }


public: /// Getter
    const Json::Value& GetObject(const std::string& _filename);


private: /// コンストラクタ ・ デストラクタ
    JsonLoader();
    ~JsonLoader() = default;


private: /// メンバ変数
    // JSONデータ <ファイル名, JSONオブジェクト>
    std::map<std::filesystem::path, Json::Value> data_;
    Json::Parser* parser_ = nullptr;
    Json::Saver* saver_ = nullptr;
};