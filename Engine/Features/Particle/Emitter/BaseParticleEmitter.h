#pragma once

#include <Timer/Timer.h>
#include <string>

struct EmitterData
{
    float           emitInterval_;                  // 発生間隔
    uint32_t        emitNum_;                       // 発生数
    float           emitterLifeTime_;               // エミッタ寿命
    Vector3         beginPosition_;                 // 発生開始位置
    Vector3         endPosition_;                   // 発生終了位置
    Vector3         emitPositionFixed_;             // ランダム発生しない場合の発生位置
    bool            enableRandomEmit_;              // ランダム発生
};

class BaseParticleEmitter
{
public:
    virtual ~BaseParticleEmitter() {};

    virtual void Initialize(const std::string& _filePath);
    virtual void Update() = 0;

public: /// Getter
    EmitterData& GetEmitterData() { return emitterData_; }

protected:
    std::string     name_;              // 名前
    Timer           timer_;             // 計測用タイマー
    EmitterData     emitterData_;       // エミッタデータ

protected:
    virtual void DebugWindow();
};