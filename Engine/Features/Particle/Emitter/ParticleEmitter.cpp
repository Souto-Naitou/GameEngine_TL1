#include "ParticleEmitter.h"

#ifdef _DEBUG
#include <DebugTools/DebugManager/DebugManager.h>
#include <DebugTools/ImGuiTemplates/ImGuiTemplates.h>
#include <sstream>
#endif // _DEBUG

#include <Features/Particle/ParticleManager.h>
#include <Features/RandomGenerator/RandomGenerator.h>
#include <Features/Particle/Manager/EmitterManager.h>

const uint32_t ParticleEmitter::kDefaultReserveCount_;

void ParticleEmitter::Initialize(const std::string& _modelPath, const std::string& _jsonPath)
{
#ifdef _DEBUG
    std::stringstream ss;
    ss << "instance##0x" << std::hex << this;
    name_ = ss.str();
    DebugManager::GetInstance()->SetComponent("ParticleEmitter", name_, std::bind(&ParticleEmitter::DebugWindow, this));
#endif // _DEBUG

    jsonPath_ = _jsonPath;

    timer_.Start();
    reloadTimer_.Start();
    particle_ = &ParticleManager::GetInstance()->CreateParticle();
    particle_->Initialize(_modelPath);
    particle_->reserve(kDefaultReserveCount_);

    emitterData_ = EmitterManager::GetInstance()->LoadFile(jsonPath_);

    aabb_ = std::make_unique<AABB>();
    aabb_->Initialize();
}

void ParticleEmitter::Update()
{
    if (timer_.GetNow() > emitterData_.emitInterval_)
    {
        if (emitterData_.emitNum_ < 0)
        {
            emitterData_.emitNum_ = 0;
        }
        for (int32_t i = 0; i < emitterData_.emitNum_; ++i)
        {
            EmitParticle();
        }
        timer_.Reset();
        timer_.Start();
    }

    //if (reloadTimer_.GetNow() > reloadInterval_)
    //{
    //    emitterData_ = EmitterManager::GetInstance()->ReloadFile(jsonPath_);
    //    reloadTimer_.Reset();
    //    reloadTimer_.Start();
    //}
}

void ParticleEmitter::Draw()
{
#ifdef _DEBUG
    if (emitterData_.enableRandomEmit_)
    {
        aabb_->Draw();
    }
#endif // _DEBUG
}

void ParticleEmitter::EmitParticle()
{
    RandomGenerator* random = RandomGenerator::GetInstance();

    particle_->emplace_back({});
    auto& parameter = particle_->GetParticleData();
    auto& datum = parameter.back();

    /// 初期トランスフォーム
    if (emitterData_.enableRandomEmit_)
    {
        datum.transform_.translate = Vector3(
            random->Generate(emitterData_.beginPosition_.x, emitterData_.endPosition_.x),
            random->Generate(emitterData_.beginPosition_.y, emitterData_.endPosition_.y),
            random->Generate(emitterData_.beginPosition_.z, emitterData_.endPosition_.z));
    }
    else
    {
        datum.transform_.translate = emitterData_.emitPositionFixed_;
    }

    /// スケール
    datum.transform_.scale = emitterData_.startScale_;
    datum.startScale_ = emitterData_.startScale_;
    datum.endScale_ = emitterData_.endScale_;
    datum.scaleDelayTime_ = emitterData_.scaleDelayTime_;

    /// ライフタイム
    datum.lifeTime_ = emitterData_.particleLifeTime_;

    /// 速度
    if (emitterData_.enableRandomVelocity_)
    {
        datum.velocity_ = Vector3(
            random->Generate(emitterData_.velocityRandomRangeBegin_.x, emitterData_.velocityRandomRangeEnd_.x),
            random->Generate(emitterData_.velocityRandomRangeBegin_.y, emitterData_.velocityRandomRangeEnd_.y),
            random->Generate(emitterData_.velocityRandomRangeBegin_.z, emitterData_.velocityRandomRangeEnd_.z)
        );
    }
    else
    {
        datum.velocity_ = emitterData_.velocityFixed_;
    }

    // 初期の色
    datum.beginColor_ = emitterData_.beginColor_;
    // 終了の色
    datum.endColor_ = emitterData_.endColor_;

    // アルファ値の変化量
    datum.alphaDeltaValue_ = emitterData_.alphaDeltaValue_;
    // 消去条件
    datum.deleteCondition_ = ParticleDeleteCondition::LifeTime;
    datum.accGravity_ = emitterData_.gravity_;
    datum.accResistance_ = emitterData_.resistance_;

    aabb_->SetMinMax(emitterData_.beginPosition_, emitterData_.endPosition_);

}

void ParticleEmitter::DebugWindow()
{
#ifdef _DEBUG

    char path[256];
    strcpy_s(path, jsonPath_.c_str());

    if (ImGui::CollapsingHeader("一般"))
    {
        if (ImGui::InputText("ファイルパス", path, sizeof(path)))
        {
            jsonFileExist_ = std::filesystem::directory_entry(path).exists();
        }
        if (ImGui::Button("保存"))
        {
            EmitterManager::GetInstance()->SaveFile(path, emitterData_);
        }
        ImGui::SameLine();
        if (ImGui::Button("読み込み"))
        {
            if (!JsonLoader::GetInstance().IsExist(path))
            {
                if (std::filesystem::directory_entry(path).exists())
                {
                    emitterData_ = EmitterManager::GetInstance()->ReloadFile(path);
                    jsonFileExist_ = true;
                }
                else
                {
                    DebugManager::GetInstance()->PushLog("ファイルが存在しません");
                    jsonFileExist_ = false;
                }
            }
        }
        if (!jsonFileExist_)
        {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "ファイルが存在しません");
        }

        ImGui::DragFloat("パーティクル寿命", &emitterData_.particleLifeTime_, 0.1f, 0.0f, FLT_MAX);
        ImGui::DragFloat("発生間隔", &emitterData_.emitInterval_, 0.02f, 0.02f, FLT_MAX);
        ImGui::InputInt("発生数", (int*)&emitterData_.emitNum_);

        ImGui::ColorEdit4("開始色", &emitterData_.beginColor_.x);
        ImGui::SameLine();
        if (ImGui::Button("入れ替え##Color"))
        {
            Vector4 temp = emitterData_.beginColor_;
            emitterData_.beginColor_ = emitterData_.endColor_;
            emitterData_.endColor_ = temp;
        }
        ImGui::ColorEdit4("終了色", &emitterData_.endColor_.x);
        ImGui::SameLine();
        if (ImGui::Button("同期##Color"))
        {
            emitterData_.endColor_ = emitterData_.beginColor_;
        }

        ImGui::SliderFloat("透明度の変化量", &emitterData_.alphaDeltaValue_, -0.2f, 0.0f);
    }

    if (ImGui::CollapsingHeader("変形"))
    {
        ImGui::DragFloat3("開始スケール", &emitterData_.startScale_.x, 0.01f);
        ImGui::SameLine();
        if (ImGui::Button("入れ替え##Scale"))
        {
            Vector3 temp = emitterData_.startScale_;
            emitterData_.startScale_ = emitterData_.endScale_;
            emitterData_.endScale_ = temp;
        }

        ImGui::DragFloat3("終了スケール", &emitterData_.endScale_.x, 0.01f);
        ImGui::SameLine();
        if (ImGui::Button("同期##Scale"))
        {
            emitterData_.endScale_ = emitterData_.startScale_;
        }
        ImGui::DragFloat("スケール遅延時間", &emitterData_.scaleDelayTime_, 0.01f);
    }

    if (ImGui::CollapsingHeader("生成場所"))
    {
        ImGui::Checkbox("ランダム範囲生成", &emitterData_.enableRandomEmit_);
        if (emitterData_.enableRandomEmit_)
        {
            ImGui::DragFloat3("発生開始地点", &emitterData_.beginPosition_.x, 0.01f);
            ImGui::DragFloat3("発生終了地点", &emitterData_.endPosition_.x, 0.01f);
        }
        else
        {
            ImGui::DragFloat3("発生位置", &emitterData_.emitPositionFixed_.x, 0.01f);
        }
    }

    if (ImGui::CollapsingHeader("速度"))
    {
        ImGui::Checkbox("速度のランダマイズ", &emitterData_.enableRandomVelocity_);
        if (emitterData_.enableRandomVelocity_)
        {
            ImGui::DragFloat3("速度ランダム範囲-開始", &emitterData_.velocityRandomRangeBegin_.x, 0.01f);
            ImGui::DragFloat3("速度ランダム範囲-終了", &emitterData_.velocityRandomRangeEnd_.x, 0.01f);
        }
        else
        {
            ImGui::DragFloat3("速度", &emitterData_.velocityFixed_.x, 0.01f);
        }
    }

    if (ImGui::CollapsingHeader("物理"))
    {
        ImGui::DragFloat3("重力", &emitterData_.gravity_.x, 0.01f);
        ImGui::DragFloat3("抵抗", &emitterData_.resistance_.x, 0.01f);
    }

    jsonPath_ = path;
#endif // _DEBUG
}

void ParticleEmitter::Finalize()
{
#ifdef _DEBUG
    DebugManager::GetInstance()->DeleteComponent("ParticleEmitter", name_.c_str());
#endif

    ParticleManager::GetInstance()->ReleaseParticle(particle_);
    particle_ = nullptr;
}