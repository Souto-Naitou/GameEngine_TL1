#pragma once
#include <unordered_map>
#include <filesystem>
#include <memory>

#include "./IModel.h"

// 読み込み済みモデル保持クラス
// - ファイルから一度読み込んだモデルのデータを保持するクラス
// - 自動読み込みに対応
class ModelStorage
{
    /// <summary>
    /// 自動読み込みパスを追加
    /// </summary>
    /// <param name="_path">パス</param>
    void AddAutoLoadPath(const std::string& _path);

    /// <summary>
    /// 全てのモデルデータを読み込む
    /// </summary>
    void LoadAllModel();

    /// <summary>
    /// モデルデータを取得
    /// </summary>
    /// <param name="_filePath">ファイルパス</param>
    /// <returns>モデルデータ</returns>
    IModel* FindModel(const std::string& _filePath);

private:
    std::unordered_map<std::filesystem::path, std::unique_ptr<IModel>> models_;
};