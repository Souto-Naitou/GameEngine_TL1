#pragma once

#include <Timer/Timer.h>
#include <string>

struct EmitterData
{
    Vector3         scale_;                         // スケール
    float           emitInterval_;                  // 発生間隔
    uint32_t        emitNum_;                       // 発生数
    float           emitterLifeTime_;               // エミッタ寿命
    Vector3         beginPosition_;                 // 発生開始位置
    Vector3         endPosition_;                   // 発生終了位置
    Vector3         emitPositionFixed_;             // ランダム発生しない場合の発生位置
    bool            enableRandomEmit_;              // ランダム発生
    Vector4         color_;                         // 色
    float           alphaDeltaValue_;               // 透明度の変化量
    bool            enableRandomVelocity_;          // ランダム速度
    Vector3         velocityRandomRangeBegin_;      // 速度ランダム範囲
    Vector3         velocityRandomRangeEnd_;        // 速度ランダム範囲
    Vector3         velocityFixed_;                 // 速度固定
    Vector3         gravity_;                       // 重力
    Vector3         resistance_;                    // 抵抗
};


class BaseParticleEmitter
{
public:
    virtual ~BaseParticleEmitter() {};

    virtual void Initialize(const std::string& _filePath);
    virtual void Update() = 0;
    virtual void Finalize() = 0;


public: /// Getter
    EmitterData& GetEmitterData() { return emitterData_; }


protected:
    std::string     name_;              // 名前
    Timer           timer_;             // 計測用タイマー
    EmitterData     emitterData_;       // エミッタデータ


protected:
    virtual void DebugWindow();
};