#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <Vector3.h>
#include <Vector4.h>
#include <Range.h>
#include <cstdint>

struct EmitterData
{
    std::string     name_                       = {};                   // 名前
    Vector3         scaleFixed_                 = {};                   // 固定スケール
    Range<Vector3>  scaleRange_                 = {};                   // スケール範囲
    Range<Vector3>  scaleRandomRange_           = {};                   // スケールランダム範囲

    float           emitInterval_               = {};                   // 発生間隔
    int32_t         emitNum_                    = {};                   // 発生数
    float           emitterLifeTime_            = {};                   // エミッタ寿命
    float           particleLifeTime_           = {};                   // パーティクル寿命
    float           scaleDelayTime_             = {};                   // スケール遅延時間
    Range<Vector3>  positionRange_              = {};                   // 発生位置範囲
    Vector3         emitPositionFixed_          = {};                   // ランダム発生しない場合の発生位置
    Range<Vector4>  colorRange_                 = {};                   // 色変化始点終点
    float           alphaDeltaValue_            = {};                   // 透明度の変化量
    Range<Vector3>  velocityRandomRange_        = {};                   // 速度ランダム範囲
    Vector3         velocityFixed_              = {};                   // 速度固定
    Range<Vector3>  rotationRandomRange_        = {};                   // 回転ランダム範囲
    Vector3         gravity_                    = {};                   // 重力
    Vector3         resistance_                 = {};                   // 抵抗
    bool            enableRandomVelocity_       = {};                   // ランダム速度
    bool            enableRandomEmit_           = {};                   // ランダム発生
    bool            enableRandomRotation_       = {};                   // ランダム回転
    bool            enableRandomScale_          = {};                   // ランダム拡縮
    bool            enableScaleTransition_      = {};                   // スケール遷移
};

void from_json(const nlohmann::json& _j, EmitterData& _data);
void to_json(nlohmann::json& _j, const EmitterData& _data);
