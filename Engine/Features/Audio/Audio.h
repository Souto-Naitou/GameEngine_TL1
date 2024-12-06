#pragma once

#include <xaudio2.h>
#include <fstream>
#include <wrl.h>

#include <Common/HRESULT_ASSERT.h>

#pragma comment(lib, "xaudio2.lib")

struct ChunkHeader
{
    char id[4];
    DWORD size;
};

struct RiffHeader
{
    ChunkHeader chunkHeader;
    char format[4];
};

struct FormatChunk
{
    ChunkHeader chunkHeader;
    WAVEFORMATEX wfex;
};

struct SoundData
{
    WAVEFORMATEX wfex;
    BYTE* pBuffer;
    DWORD bufferSize;
};

class Audio
{
public:
    void Initialize();
    void Finalize();

    SoundData LoadWave(const char* _filename);
    void Unload(SoundData* soundData);

    void PlayWave(IXAudio2* xAudio2, const SoundData& soundData);

private:
    HRESULT_ASSERT hr_ = {};
    Microsoft::WRL::ComPtr<IXAudio2> pXAudio2_;
    IXAudio2MasteringVoice* pMasteringVoice_ = nullptr;
};
