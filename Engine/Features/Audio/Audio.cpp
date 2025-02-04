#include "Audio.h"

#include <fstream>
#include <cassert>
#include <memory>

void Audio::Initialize()
{
    hr_ = XAudio2Create(&pXAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    hr_ = pXAudio2_->CreateMasteringVoice(&pMasteringVoice_);
}

void Audio::Finalize()
{
    pXAudio2_.Reset();
}

SoundData Audio::LoadWave(const char* _filename)
{
    std::ifstream file(_filename, std::ios::binary);

    assert(file.is_open());

    /// RIFFチャンク読み込み
    RiffHeader riff = {};
    file.read(reinterpret_cast<char*>(&riff), sizeof(riff));
    /// riffチェック
    if (strncmp(riff.chunkHeader.id, "RIFF", 4) != 0)
    {
        assert(false);
    }
    /// Waveチェック
    if (strncmp(riff.format, "WAVE", 4) != 0)
    {
        assert(false);
    }


    /// Formatチャンク読み込み
    FormatChunk format = {};
    // ヘッダー読み込み
    file.read(reinterpret_cast<char*>(&format), sizeof(ChunkHeader));
    if (strncmp(format.chunkHeader.id, "fmt ", 4) != 0)
    {
        assert(false);
    }
    // フォーマット読み込み
    assert(format.chunkHeader.size <= sizeof(format.wfex));
    file.read(reinterpret_cast<char*>(&format.wfex), format.chunkHeader.size);


    /// Dataチャンク読み込み
    ChunkHeader data = {};
    file.read(reinterpret_cast<char*>(&data), sizeof(data));
    // JUNKチャンクがある場合はスキップ
    if (strncmp(data.id, "JUNK", 4) == 0)
    {
        file.seekg(data.size, std::ios::cur);
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
    }
    if (strncmp(data.id, "data", 4) != 0)
    {
        assert(false);
    }

    /// 波形データ読み込み
    std::unique_ptr<char[]> pBuffer = std::make_unique<char[]>(data.size);
    file.read(pBuffer.get(), data.size);


    /// ファイルクローズ
    file.close();


    SoundData soundData = {};
    soundData.wfex = format.wfex;
    soundData.pBuffer = std::make_unique<BYTE[]>(data.size);
    soundData.bufferSize = data.size;

    std::memcpy(soundData.pBuffer.get(), pBuffer.get(), data.size);

    return soundData;
}

void Audio::Unload(SoundData* soundData)
{
    soundData->pBuffer.reset();
    soundData->bufferSize = 0;
    soundData->wfex = {};

    return;
}

void Audio::PlayWave(IXAudio2* xAudio2, const SoundData& soundData)
{
    /// SourceVoice作成
    IXAudio2SourceVoice* pSourceVoice = nullptr;
    hr_ = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);


    /// バッファ設定
    XAUDIO2_BUFFER buffer = {};
    buffer.pAudioData = soundData.pBuffer.get();
    buffer.AudioBytes = soundData.bufferSize;
    buffer.Flags = XAUDIO2_END_OF_STREAM;

    /// 再生
    hr_ = pSourceVoice->SubmitSourceBuffer(&buffer);
    hr_ = pSourceVoice->Start(0);
}
