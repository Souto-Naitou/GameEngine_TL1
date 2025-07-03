#pragma once
#include <mutex>
#include <list>


// モデル読み込み機能クラス
// - ファイルからモデルを生成して返す
// - モデルを保持すること
class ModelLoader
{
public:
    void Initialize();
    void Update();

    /// <summary>
    /// 検索パスを追加
    /// </summary>
    /// <param name="_path">パス</param>
    void AddSearchPath(const std::string& _path);

    /// <summary>
    /// 検索パスを取得
    /// </summary>
    /// <returns>検索パス</returns>
    std::string GetDirectoryPath(std::string _fileName);

    /// <summary>
    /// モデルとテクスチャを指定されたファイルパスから読み込みます
    /// </summary>
    /// <param name="_filePath">モデルファイルのパス</param>
    /// <param name="_texturePath">テクスチャファイルのパス</param>
    void LoadModel(const std::string& _filePath, const std::string& _texturePath = {});

    /// <summary>
    /// アップロードキューに登録
    /// </summary>
    /// <param name="_pFunc">関数ポインタ</param>
    void InqueueUpload(Model* _ptr) 
    {
        std::lock_guard<std::mutex> lock(mtx_);
        uploadQueue_.push(_ptr); 
    }

private:
    mutable std::mutex mtx_;

    std::list<std::string> searchPaths_;
    std::list<std::string> autoLoadPaths_;
};