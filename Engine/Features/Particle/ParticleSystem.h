#pragma once

#include <wrl.h>
#include <Core/DirectX12/DirectX12.h>
#include <Features/GameEye/GameEye.h>

class ParticleSystem
{
public:
    void Initialize();
    void PresentDraw();
    void Finalize();


public: /// Setter
    void SetDefaultGameEye(GameEye* _pGameEye) { pDefaultGameEye_ = _pGameEye; }


public: /// Getter
    GameEye* GetDefaultGameEye() const { return pDefaultGameEye_; }


public: /// 公開定数
    static constexpr uint32_t kMaxInstance_ = 256u;


private: /// メンバ変数
    static constexpr wchar_t kVertexShaderPath[] = L"EngineResources/Shaders/Particle.VS.hlsl";
    static constexpr wchar_t kPixelShaderPath[] = L"EngineResources/Shaders/Particle.PS.hlsl";
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
    GameEye* pDefaultGameEye_ = nullptr;


private: /// 非公開関数
    void CreateRootSignature();
    void CreatePipelineState();


private:
    DirectX12* pDx12_;
};