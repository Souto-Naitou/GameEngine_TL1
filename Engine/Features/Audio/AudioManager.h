#pragma once

#include "Audio.h"
#include <Utility/FilePathSearcher/FilePathSearcher.h>
#include <map>
#include <list>
#include <string>

class AudioManager
{
public:
    AudioManager(AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;

    static AudioManager* GetInstance()
    {
        static AudioManager instance;
        return &instance;
    }


public:
    void Initialize();
    void Update();
    void Finalize();

    /// <summary>
    /// 検索パスを追加
    /// </summary>
    /// <param name="_path">パス</param>
    void AddSearchPath(const std::string& _path);

    void AddSourceVoice(IXAudio2SourceVoice* _sv) { sourceVoices_.push_back(_sv); }
    Audio* GetNewAudio(const std::string& _filename);


private:
    HRESULT_ASSERT hr_ = {};
    Microsoft::WRL::ComPtr<IXAudio2> pXAudio2_;
    IXAudio2MasteringVoice* pMasteringVoice_ = nullptr;
    std::map<std::string, SoundData> soundDataMap_;
    std::unique_ptr<FilePathSearcher> pFilePathSearcher_ = nullptr;

    std::list<IXAudio2SourceVoice*> sourceVoices_;

private:
    AudioManager() = default;
    ~AudioManager() = default;
    SoundData& LoadWave(const char* _filename);

    void ChunkHeaderRead(std::ifstream& _file, ChunkHeader& _chunkHeader, const char* _target);
};