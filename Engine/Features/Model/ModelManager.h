#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <list>
#include <queue>
#include "Model.h"
#include <filesystem>
#include <mutex>
#include <Core/DirectX12/DirectX12.h>

class Particle;

class ModelManager : public EngineFeature
{
public:
    ModelManager(ModelManager&) = delete;
    ModelManager(ModelManager&&) = delete;
    ModelManager& operator=(ModelManager&) = delete;
    ModelManager& operator=(ModelManager&&) = delete;

    static ModelManager* GetInstance()
    {
        static ModelManager instance;
        return &instance;
    }

    void Initialize();
    void Update();

    /// <summary>
    /// 検索パスを追加
    /// </summary>
    /// <param name="_path">パス</param>
    void AddSearchPath(const std::string& _path);

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
    /// モデルデータを取得
    /// </summary>
    /// <param name="_filePath">ファイルパス</param>
    /// <returns>モデルデータ</returns>
    Model* FindModel(const std::string& _filePath);

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

    std::unordered_map<std::filesystem::path, std::unique_ptr<Model>> models_;

    std::queue<Model*> uploadQueue_;
    std::queue<Particle*> uploadQueueParticle_;

    ModelManager() = default;
    ~ModelManager() = default;

    std::filesystem::path GetLowerPath(const std::string& _path);
};