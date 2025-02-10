#include "Audio.h"

#include "AudioManager.h"

#include <fstream>
#include <cassert>
#include <memory>

void Audio::Initialize()
{
}

void Audio::Finalize()
{

}

void Audio::Unload(SoundData* soundData)
{
    soundData->pBuffer.reset();
    soundData->bufferSize = 0;
    soundData->wfex = {};

    return;
}

void Audio::Play()
{
    /// SourceVoice作成
    hr_ = pXAudio2_->CreateSourceVoice(&pCurrentSourceVoice_, &soundData_->wfex);

    /// バッファ設定
    XAUDIO2_BUFFER buffer = {};
    buffer.pAudioData = soundData_->pBuffer.get();
    buffer.AudioBytes = soundData_->bufferSize;
    buffer.Flags = XAUDIO2_END_OF_STREAM;

    /// 再生
    hr_ = pCurrentSourceVoice_->SubmitSourceBuffer(&buffer);
    hr_ = pCurrentSourceVoice_->Start(0);

    AudioManager::GetInstance()->AddSourceVoice(pCurrentSourceVoice_);
}
