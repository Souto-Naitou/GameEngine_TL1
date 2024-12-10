#pragma once

#include <Timer/Timer.h>
#include <string>

struct EmitterData
{
    Vector3         startScale_;                    // 開始スケール
    Vector3         endScale_;                      // 終了スケール

    float           emitInterval_;                  // 発生間隔
    int32_t         emitNum_;                       // 発生数
    float           emitterLifeTime_;               // エミッタ寿命
    float           particleLifeTime_;              // パーティクル寿命
    float           scaleDelayTime_;                // スケール遅延時間
    Vector3         beginPosition_;                 // 発生開始位置
    Vector3         endPosition_;                   // 発生終了位置
    Vector3         emitPositionFixed_;             // ランダム発生しない場合の発生位置
    Vector4         beginColor_;                    // 開始色
    Vector4         endColor_;                      // 開始色
    float           alphaDeltaValue_;               // 透明度の変化量
    Vector3         velocityRandomRangeBegin_;      // 速度ランダム範囲
    Vector3         velocityRandomRangeEnd_;        // 速度ランダム範囲
    Vector3         velocityFixed_;                 // 速度固定
    Vector3         gravity_;                       // 重力
    Vector3         resistance_;                    // 抵抗
    bool            enableRandomVelocity_;          // ランダム速度
    bool            enableRandomEmit_;              // ランダム発生

};


class BaseParticleEmitter
{
public:
    virtual ~BaseParticleEmitter() {};

    virtual void Initialize(const std::string& _filePath);
    virtual void Update() = 0;
    virtual void Finalize();


public: /// Getter
    EmitterData& GetEmitterData() { return emitterData_; }


protected:
    std::string     name_;              // 名前
    Timer           timer_;             // 計測用タイマー
    EmitterData     emitterData_;       // エミッタデータ


protected:
    virtual void DebugWindow();
};