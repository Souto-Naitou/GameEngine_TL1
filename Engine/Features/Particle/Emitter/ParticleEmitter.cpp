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


    if (!emitterData_.name.empty())
    {
        particleName_ = emitterData_.name;
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
    if (timer_.GetNow<float>() > emitterData_.common.emitInterval)
    {
        if (emitterData_.common.emitNum < 0)
        {
            emitterData_.common.emitNum = 0;
        }
        for (int32_t i = 0; i < emitterData_.common.emitNum; ++i)
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
    emitterData_.common.emitPositionFixed = fromJsonData_.common.emitPositionFixed + position_;
    emitterData_.ranges.position.start() = fromJsonData_.ranges.position.start() + position_;
    emitterData_.ranges.position.end() = fromJsonData_.ranges.position.end() + position_;

    aabb_->SetMinMax(emitterData_.ranges.position.start(), emitterData_.ranges.position.end());
}

void ParticleEmitter::Draw()
{
#ifdef _DEBUG
    if (emitterData_.flags.enableRandomEmit)
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
    if (emitterData_.flags.enableRandomEmit)
    {
        datum.transform_.translate = Vector3(
            random->Generate(emitterData_.ranges.position.start().x, emitterData_.ranges.position.end().x),
            random->Generate(emitterData_.ranges.position.start().y, emitterData_.ranges.position.end().y),
            random->Generate(emitterData_.ranges.position.start().z, emitterData_.ranges.position.end().z));
    }
    else
    {
        datum.transform_.translate = emitterData_.common.emitPositionFixed;
    }

    /// スケール
    if (emitterData_.flags.enableRandomScale)
    {
        datum.transform_.scale = Vector3(
            random->Generate(emitterData_.ranges.scaleRandom.start().x, emitterData_.ranges.scaleRandom.end().x),
            random->Generate(emitterData_.ranges.scaleRandom.start().y, emitterData_.ranges.scaleRandom.end().y),
            random->Generate(emitterData_.ranges.scaleRandom.start().z, emitterData_.ranges.scaleRandom.end().z)
        );
        datum.scaleRange_.start() = datum.transform_.scale;
        if (!emitterData_.flags.enableScaleTransition)
        {
            datum.scaleRange_.end() = datum.transform_.scale;
        }
    }
    else if (emitterData_.flags.enableScaleTransition)
    {
        datum.transform_.scale = emitterData_.ranges.scale.start();
        datum.scaleRange_ = emitterData_.ranges.scale;
    }
    else
    {
        datum.transform_.scale = emitterData_.common.scaleFixed;
        datum.scaleRange_ = Range(emitterData_.common.scaleFixed, emitterData_.common.scaleFixed);
    }
    datum.scaleDelayTime_ = emitterData_.common.scaleDelayTime;

    /// ライフタイム
    datum.lifeTime_ = emitterData_.common.particleLifeTime;

    /// 速度
    if (emitterData_.flags.enableRandomVelocity)
    {
        datum.velocity_ = Vector3(
            random->Generate(emitterData_.ranges.velocityRandom.start().x, emitterData_.ranges.velocityRandom.end().x),
            random->Generate(emitterData_.ranges.velocityRandom.start().y, emitterData_.ranges.velocityRandom.end().y),
            random->Generate(emitterData_.ranges.velocityRandom.start().z, emitterData_.ranges.velocityRandom.end().z)
        );
    }
    else
    {
        datum.velocity_ = emitterData_.common.velocityFixed;
    }

    // 回転
    if (emitterData_.flags.enableRandomRotation)
    {
        datum.transform_.rotate = Vector3(
            random->Generate(emitterData_.ranges.rotationRandom.start().x, emitterData_.ranges.rotationRandom.end().x),
            random->Generate(emitterData_.ranges.rotationRandom.start().y, emitterData_.ranges.rotationRandom.end().y),
            random->Generate(emitterData_.ranges.rotationRandom.start().z, emitterData_.ranges.rotationRandom.end().z)
        );
    }
    else
    {
        datum.transform_.rotate = {};
    }

    // 色範囲
    datum.colorRange_ = emitterData_.ranges.color;

    // アルファ値の変化量
    datum.alphaDeltaValue_ = emitterData_.common.alphaDeltaValue;
    // 消去条件
    datum.deleteCondition_ = ParticleDeleteCondition::LifeTime;
    datum.accGravity_ = emitterData_.physics.gravity;
    datum.accResistance_ = emitterData_.physics.resistance;

    aabb_->SetMinMax(emitterData_.ranges.position.start(), emitterData_.ranges.position.end());
}

void ParticleEmitter::DebugWindow()
{
#ifdef _DEBUG

    char path[512] = "";
    char name[128] = "";
    memcpy_s(path, sizeof(name), jsonPath_.c_str(), jsonPath_.size());
    memcpy_s(name, sizeof(name), fromJsonData_.name.c_str(), fromJsonData_.name.size());

    ImGui::Text("Name : %s", particleName_.c_str());
    if (ImGui::CollapsingHeader("一般"))
    {
        if (ImGui::InputText("エミッター名", name, sizeof(name), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            particleName_ = name;
            name_ = particleName_ + "##" + ptrHex_;
            fromJsonData_.name = name;
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
                if (!fromJsonData_.name.empty())
                {
                    name_ = fromJsonData_.name;
                    name_ = fromJsonData_.name + "##" + ptrHex_;
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

        ImGui::DragFloat("パーティクル寿命", &fromJsonData_.common.particleLifeTime, 0.1f, 0.0f, FLT_MAX);

        ImGui::DragFloat("発生間隔", &fromJsonData_.common.emitInterval, 0.02f, 0.02f, FLT_MAX);

        ImGui::InputInt("発生数", (int*)&fromJsonData_.common.emitNum);

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("色の変化"))
    {
        ImGui::ColorEdit4("開始色", &fromJsonData_.ranges.color.start().x);
        ImGui::SameLine();
        if (ImGui::Button("入れ替え##Color"))
        {
            Vector4 temp = fromJsonData_.ranges.color.start();
            fromJsonData_.ranges.color.start() = fromJsonData_.ranges.color.end();
            fromJsonData_.ranges.color.end() = temp;
        }

        ImGui::ColorEdit4("終了色", &fromJsonData_.ranges.color.end().x);
        ImGui::SameLine();
        if (ImGui::Button("同期##Color"))
        {
            fromJsonData_.ranges.color.end() = fromJsonData_.ranges.color.start();
        }

        ImGui::SliderFloat("透明度の変化量", &fromJsonData_.common.alphaDeltaValue, -0.2f, 0.0f);


        ImGui::Spacing();
    }
    
    if (ImGui::CollapsingHeader("変形"))
    {
        if (!fromJsonData_.flags.enableRandomScale && !fromJsonData_.flags.enableScaleTransition)
        {
            ImGui::DragFloat3("スケール", &fromJsonData_.common.scaleFixed.x, 0.01f);
        }

        ImGui::Checkbox("スケールのランダマイズ", &fromJsonData_.flags.enableRandomScale);
        if (fromJsonData_.flags.enableRandomScale)
        {
            ImGui::DragFloat3("スケールランダム範囲-開始", &fromJsonData_.ranges.scaleRandom.start().x, 0.01f);
            ImGui::DragFloat3("スケールランダム範囲-終了", &fromJsonData_.ranges.scaleRandom.end().x, 0.01f);
        }

        ImGui::Checkbox("スケール遷移", &fromJsonData_.flags.enableScaleTransition);
        if (fromJsonData_.flags.enableScaleTransition)
        {
            if (fromJsonData_.flags.enableRandomScale) ImGui::BeginDisabled();
            ImGui::DragFloat3("開始スケール", &fromJsonData_.ranges.scale.start().x, 0.01f);
            if (fromJsonData_.flags.enableRandomScale) ImGui::EndDisabled();

            ImGui::SameLine();
            if (ImGui::Button("入れ替え##Scale"))
            {
                Vector3 temp = fromJsonData_.ranges.scale.start();
                fromJsonData_.ranges.scale.start() = fromJsonData_.ranges.scale.end();
                fromJsonData_.ranges.scale.end() = temp;
            }

            ImGui::DragFloat3("終了スケール", &fromJsonData_.ranges.scale.end().x, 0.01f);
            ImGui::SameLine();
            if (ImGui::Button("同期##Scale"))
            {
                fromJsonData_.ranges.scale.end() = fromJsonData_.ranges.scale.start();
            }
            ImGui::DragFloat("スケール遅延時間", &fromJsonData_.common.scaleDelayTime, 0.01f);
        }

        ImGui::Checkbox("姿勢のランダマイズ", &fromJsonData_.flags.enableRandomRotation);
        if (fromJsonData_.flags.enableRandomRotation)
        {
            ImGui::DragFloat3("姿勢ランダム範囲-開始", &fromJsonData_.ranges.rotationRandom.start().x, 0.01f);
            ImGui::DragFloat3("姿勢ランダム範囲-終了", &fromJsonData_.ranges.rotationRandom.end().x, 0.01f);
        }

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("生成場所"))
    {
        ImGui::Checkbox("ランダム範囲生成", &fromJsonData_.flags.enableRandomEmit);
        if (fromJsonData_.flags.enableRandomEmit)
        {
            ImGui::DragFloat3("発生開始地点", &fromJsonData_.ranges.position.start().x, 0.01f);
            ImGui::DragFloat3("発生終了地点", &fromJsonData_.ranges.position.end().x, 0.01f);
        }
        else
        {
            ImGui::DragFloat3("発生位置", &fromJsonData_.common.emitPositionFixed.x, 0.01f);
        }

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("速度"))
    {
        ImGui::Checkbox("速度のランダマイズ", &fromJsonData_.flags.enableRandomVelocity);
        if (fromJsonData_.flags.enableRandomVelocity)
        {
            ImGui::DragFloat3("速度ランダム範囲-開始", &fromJsonData_.ranges.velocityRandom.start().x, 0.01f);
            ImGui::DragFloat3("速度ランダム範囲-終了", &fromJsonData_.ranges.velocityRandom.end().x, 0.01f);
        }
        else
        {
            ImGui::DragFloat3("速度", &fromJsonData_.common.velocityFixed.x, 0.01f);
        }

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("物理"))
    {
        ImGui::DragFloat3("重力", &fromJsonData_.physics.gravity.x, 0.01f);
        ImGui::DragFloat3("抵抗", &fromJsonData_.physics.resistance.x, 0.01f);

    }

    jsonPath_ = path;
#endif // _DEBUG
}

void ParticleEmitter::ModifyGameEye(GameEye** _eye)
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
        for (int32_t i = 0; i < emitterData_.common.emitNum; ++i)
        {
            EmitParticle();
        }
    }
}