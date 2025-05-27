#include "ParticleEmitter.h"

#ifdef _DEBUG
#include <DebugTools/DebugManager/DebugManager.h>
#include <imgui.h>
#include <Utility/String/strutl.h>
#endif // _DEBUG

#include <Features/Particle/ParticleManager.h>
#include <Features/RandomGenerator/RandomGenerator.h>
#include <Features/Particle/Manager/EmitterManager.h>
#include "EmitterData.h"
#include <WinTools/WinTools.h>
#include <Range.h>

const uint32_t ParticleEmitter::kDefaultReserveCount_;

void ParticleEmitter::Initialize(const std::string& _modelPath, const std::string& _texturePath, const std::string& _jsonPath)
{
#ifdef _DEBUG
    ptrHex_ = utl::string::to_string(this);
    name_ = "unnamed##" + ptrHex_;
    DebugManager::GetInstance()->SetComponent("ParticleEmitter", name_, std::bind(&ParticleEmitter::DebugWindow, this));
#endif // _DEBUG

    winTools_ = WinTools::GetInstance();

    jsonPath_ = _jsonPath;

    // タイマースタート
    timer_.Start();
    reloadTimer_.Start();

    // パーティクル初期化
    particle_ = ParticleManager::GetInstance()->CreateParticle();
    particle_->Initialize(_modelPath, _texturePath);
    particle_->reserve(kDefaultReserveCount_);

    if (jsonPath_.empty())
    {
        fromJsonData_ = {};
        emitterData_ = {};
    }
    else
    {
        fromJsonData_ = EmitterManager::GetInstance()->LoadFile(jsonPath_);
        emitterData_ = fromJsonData_;
    }


    if (!emitterData_.name_.empty())
    {
        particleName_ = emitterData_.name_;
        name_ = particleName_ + "##" + ptrHex_;
    }

    aabb_ = std::make_unique<AABB>();
    aabb_->Initialize();
}

void ParticleEmitter::EnableManualMode()
{
    isManualMode_ = true;
}

void ParticleEmitter::DisableManualMode()
{
    isManualMode_ = false;
}

void ParticleEmitter::Update()
{
    if (timer_.GetNow<float>() > emitterData_.emitInterval_)
    {
        if (emitterData_.emitNum_ < 0)
        {
            emitterData_.emitNum_ = 0;
        }
        for (int32_t i = 0; i < emitterData_.emitNum_; ++i)
        {
            if (!isManualMode_)
            {
                EmitParticle();
            }
        }
        isEmitRequest_ = false;
        timer_.Reset();
        timer_.Start();
    }

    emitterData_ = fromJsonData_;
    emitterData_.emitPositionFixed_ = fromJsonData_.emitPositionFixed_ + position_;
    emitterData_.positionRange_.start() = fromJsonData_.positionRange_.start() + position_;
    emitterData_.positionRange_.end() = fromJsonData_.positionRange_.end() + position_;

    aabb_->SetMinMax(emitterData_.positionRange_.start(), emitterData_.positionRange_.end());
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
            random->Generate(emitterData_.positionRange_.start().x, emitterData_.positionRange_.end().x),
            random->Generate(emitterData_.positionRange_.start().y, emitterData_.positionRange_.end().y),
            random->Generate(emitterData_.positionRange_.start().z, emitterData_.positionRange_.end().z));
    }
    else
    {
        datum.transform_.translate = emitterData_.emitPositionFixed_;
    }

    /// スケール
    if (emitterData_.enableRandomScale_)
    {
        datum.transform_.scale = Vector3(
            random->Generate(emitterData_.scaleRandomRange_.start().x, emitterData_.scaleRandomRange_.end().x),
            random->Generate(emitterData_.scaleRandomRange_.start().y, emitterData_.scaleRandomRange_.end().y),
            random->Generate(emitterData_.scaleRandomRange_.start().z, emitterData_.scaleRandomRange_.end().z)
        );
        datum.scaleRange_.start() = datum.transform_.scale;
        if (!emitterData_.enableScaleTransition_)
        {
            datum.scaleRange_.end() = datum.transform_.scale;
        }
    }
    else if (emitterData_.enableScaleTransition_)
    {
        datum.transform_.scale = emitterData_.scaleRange_.start();
        datum.scaleRange_ = emitterData_.scaleRange_;
    }
    else
    {
        datum.transform_.scale = emitterData_.scaleFixed_;
        datum.scaleRange_ = Range(emitterData_.scaleFixed_, emitterData_.scaleFixed_);
    }
    datum.scaleDelayTime_ = emitterData_.scaleDelayTime_;

    /// ライフタイム
    datum.lifeTime_ = emitterData_.particleLifeTime_;

    /// 速度
    if (emitterData_.enableRandomVelocity_)
    {
        datum.velocity_ = Vector3(
            random->Generate(emitterData_.velocityRandomRange_.start().x, emitterData_.velocityRandomRange_.end().x),
            random->Generate(emitterData_.velocityRandomRange_.start().y, emitterData_.velocityRandomRange_.end().y),
            random->Generate(emitterData_.velocityRandomRange_.start().z, emitterData_.velocityRandomRange_.end().z)
        );
    }
    else
    {
        datum.velocity_ = emitterData_.velocityFixed_;
    }

    // 回転
    if (emitterData_.enableRandomRotation_)
    {
        datum.transform_.rotate = Vector3(
            random->Generate(emitterData_.rotationRandomRange_.start().x, emitterData_.rotationRandomRange_.end().x),
            random->Generate(emitterData_.rotationRandomRange_.start().y, emitterData_.rotationRandomRange_.end().y),
            random->Generate(emitterData_.rotationRandomRange_.start().z, emitterData_.rotationRandomRange_.end().z)
        );
    }
    else
    {
        datum.transform_.rotate = {};
    }

    // 色範囲
    datum.colorRange_ = emitterData_.colorRange_;

    // アルファ値の変化量
    datum.alphaDeltaValue_ = emitterData_.alphaDeltaValue_;
    // 消去条件
    datum.deleteCondition_ = ParticleDeleteCondition::LifeTime;
    datum.accGravity_ = emitterData_.gravity_;
    datum.accResistance_ = emitterData_.resistance_;

    aabb_->SetMinMax(emitterData_.positionRange_.start(), emitterData_.positionRange_.end());
}

void ParticleEmitter::DebugWindow()
{
#ifdef _DEBUG

    char path[512] = "";
    char name[128] = "";
    std::memcpy(path, jsonPath_.c_str(), jsonPath_.size());
    std::memcpy(name, fromJsonData_.name_.c_str(), fromJsonData_.name_.size());

    ImGui::Text("Name : %s", particleName_.c_str());
    if (ImGui::CollapsingHeader("一般"))
    {
        if (ImGui::InputText("エミッター名", name, sizeof(name)))
        {
            particleName_ = name;
            name_ = particleName_ + "##" + ptrHex_;
            fromJsonData_.name_ = name;
        }
        if (ImGui::Button("マニュアル発生"))
        {
            EmitParticle();
        }
        if (ImGui::InputText("ファイルパス", path, sizeof(path)))
        {
            jsonFileExist_ = std::filesystem::directory_entry(path).exists();
        }

        if (ImGui::Button("保存"))
        {
            EmitterManager::GetInstance()->SaveFile(path, fromJsonData_);
        }
        ImGui::SameLine();
        if (ImGui::Button("読み込み"))
        {
            if (std::filesystem::directory_entry(path).exists())
            {
                fromJsonData_ = EmitterManager::GetInstance()->ReloadFile(path);
                /// 名前が空でないなら
                if (!fromJsonData_.name_.empty())
                {
                    name_ = fromJsonData_.name_;
                    name_ = fromJsonData_.name_ + "##" + ptrHex_;
                }
                jsonFileExist_ = true;
            }
            else
            {
                DebugManager::GetInstance()->PushLog("ファイルが存在しません");
                jsonFileExist_ = false;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("参照"))
        {
            std::string temp = winTools_->OpenFileDialog();
            if (!temp.empty() && std::filesystem::path(temp).extension() == ".json")
            {
                std::memcpy(path, temp.c_str(), temp.size());
                jsonFileExist_ = true;
            }
            else jsonFileExist_ = false;
        }
        if (!jsonFileExist_)
        {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "ファイルが存在しないか、拡張子がjsonではありません。");
        }

        ImGui::DragFloat("パーティクル寿命", &fromJsonData_.particleLifeTime_, 0.1f, 0.0f, FLT_MAX);

        ImGui::DragFloat("発生間隔", &fromJsonData_.emitInterval_, 0.02f, 0.02f, FLT_MAX);

        ImGui::InputInt("発生数", (int*)&fromJsonData_.emitNum_);

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("色の変化"))
    {
        ImGui::ColorEdit4("開始色", &fromJsonData_.colorRange_.start().x);
        ImGui::SameLine();
        if (ImGui::Button("入れ替え##Color"))
        {
            Vector4 temp = fromJsonData_.colorRange_.start();
            fromJsonData_.colorRange_.start() = fromJsonData_.colorRange_.end();
            fromJsonData_.colorRange_.end() = temp;
        }

        ImGui::ColorEdit4("終了色", &fromJsonData_.colorRange_.end().x);
        ImGui::SameLine();
        if (ImGui::Button("同期##Color"))
        {
            fromJsonData_.colorRange_.end() = fromJsonData_.colorRange_.start();
        }

        ImGui::SliderFloat("透明度の変化量", &fromJsonData_.alphaDeltaValue_, -0.2f, 0.0f);


        ImGui::Spacing();
    }
    
    if (ImGui::CollapsingHeader("変形"))
    {
        if (!fromJsonData_.enableRandomScale_ && !fromJsonData_.enableScaleTransition_)
        {
            ImGui::DragFloat3("スケール", &fromJsonData_.scaleFixed_.x, 0.01f);
        }

        ImGui::Checkbox("スケールのランダマイズ", &fromJsonData_.enableRandomScale_);
        if (fromJsonData_.enableRandomScale_)
        {
            ImGui::DragFloat3("スケールランダム範囲-開始", &fromJsonData_.scaleRandomRange_.start().x, 0.01f);
            ImGui::DragFloat3("スケールランダム範囲-終了", &fromJsonData_.scaleRandomRange_.end().x, 0.01f);
        }

        ImGui::Checkbox("スケール遷移", &fromJsonData_.enableScaleTransition_);
        if (fromJsonData_.enableScaleTransition_)
        {
            if (fromJsonData_.enableRandomScale_) ImGui::BeginDisabled();
            ImGui::DragFloat3("開始スケール", &fromJsonData_.scaleRange_.start().x, 0.01f);
            if (fromJsonData_.enableRandomScale_) ImGui::EndDisabled();

            ImGui::SameLine();
            if (ImGui::Button("入れ替え##Scale"))
            {
                Vector3 temp = fromJsonData_.scaleRange_.start();
                fromJsonData_.scaleRange_.start() = fromJsonData_.scaleRange_.end();
                fromJsonData_.scaleRange_.end() = temp;
            }

            ImGui::DragFloat3("終了スケール", &fromJsonData_.scaleRange_.end().x, 0.01f);
            ImGui::SameLine();
            if (ImGui::Button("同期##Scale"))
            {
                fromJsonData_.scaleRange_.end() = fromJsonData_.scaleRange_.start();
            }
            ImGui::DragFloat("スケール遅延時間", &fromJsonData_.scaleDelayTime_, 0.01f);
        }

        ImGui::Checkbox("姿勢のランダマイズ", &fromJsonData_.enableRandomRotation_);
        if (fromJsonData_.enableRandomRotation_)
        {
            ImGui::DragFloat3("姿勢ランダム範囲-開始", &fromJsonData_.rotationRandomRange_.start().x, 0.01f);
            ImGui::DragFloat3("姿勢ランダム範囲-終了", &fromJsonData_.rotationRandomRange_.end().x, 0.01f);
        }

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("生成場所"))
    {
        ImGui::Checkbox("ランダム範囲生成", &fromJsonData_.enableRandomEmit_);
        if (fromJsonData_.enableRandomEmit_)
        {
            ImGui::DragFloat3("発生開始地点", &fromJsonData_.positionRange_.start().x, 0.01f);
            ImGui::DragFloat3("発生終了地点", &fromJsonData_.positionRange_.end().x, 0.01f);
        }
        else
        {
            ImGui::DragFloat3("発生位置", &fromJsonData_.emitPositionFixed_.x, 0.01f);
        }

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("速度"))
    {
        ImGui::Checkbox("速度のランダマイズ", &fromJsonData_.enableRandomVelocity_);
        if (fromJsonData_.enableRandomVelocity_)
        {
            ImGui::DragFloat3("速度ランダム範囲-開始", &fromJsonData_.velocityRandomRange_.start().x, 0.01f);
            ImGui::DragFloat3("速度ランダム範囲-終了", &fromJsonData_.velocityRandomRange_.end().x, 0.01f);
        }
        else
        {
            ImGui::DragFloat3("速度", &fromJsonData_.velocityFixed_.x, 0.01f);
        }

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("物理"))
    {
        ImGui::DragFloat3("重力", &fromJsonData_.gravity_.x, 0.01f);
        ImGui::DragFloat3("抵抗", &fromJsonData_.resistance_.x, 0.01f);

    }

    jsonPath_ = path;
#endif // _DEBUG
}

void ParticleEmitter::ModifyGameEye(GameEye* _eye)
{
    particle_->SetGameEye(_eye);
    aabb_->SetGameEye(_eye);
}

void ParticleEmitter::Finalize()
{
#ifdef _DEBUG
    DebugManager::GetInstance()->DeleteComponent("ParticleEmitter", name_.c_str());
#endif

    ParticleManager::GetInstance()->ReserveDeleteParticle(particle_);
    particle_ = nullptr;
}

void ParticleEmitter::Emit()
{
    if (isManualMode_)
    {
        this->Update();
        for (int32_t i = 0; i < emitterData_.emitNum_; ++i)
        {
            EmitParticle();
        }
    }
}