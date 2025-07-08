#pragma once
#include "./IModelLoader.h"
#include <Features/Model/IModel.h>
#include <Common/structs.h>
#include <Features/ThreadPool/ThreadPool.h>

#include <mutex>
#include <list>
#include <memory>
#include <Core/DirectX12/DirectX12.h>
#include <queue>
#include <thread>

// モデル読み込み機能クラス
// - ファイルからモデルを生成して返す
class ModelLoader : 
    public IModelLoader,
    public EngineFeature
{
public:
    void Initialize() override;
    void Update() override;

    /// 検索パスを追加
    void AddSearchPath(const std::string& _path);
    /// 自動読み込みパスを追加
    void AddAutoLoadPath(const std::string& _path);
    /// 検索パスを取得
    [[nodiscard]] std::string GetDirectoryPath(const std::string& _fileName);
    /// モデルとテクスチャを指定されたファイルパスから読み込み
    std::shared_ptr<IModel> LoadModel(const std::string& _path) override;
    /// 全てのモデルデータを読み込む
    void LoadModelAll();

private:
    /// アップロードキューに登録
    void EnqueueUpload(IModel* _ptr);

private:
    mutable std::mutex mtx_;

    std::list<std::string> searchPaths_;
    std::list<std::string> autoLoadPaths_;

    std::queue<IModel*> uploadQueue_;
    
    // Pointers
    ThreadPool* pThreadPool_ = nullptr;
};