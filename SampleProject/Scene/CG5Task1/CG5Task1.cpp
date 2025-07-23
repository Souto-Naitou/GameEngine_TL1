#include "CG5Task1.h"
#include <Features/Model/ObjModel.h>
#include <Features/GameEye/FreeLook/FreeLookEye.h>

void CG5Task1::Initialize()
{
    pLogger_ = Logger::GetInstance();
    pTextureManager_ = TextureManager::GetInstance();

    this->_DecomposeArg();

    pGameEye_ = std::make_unique<FreeLookEye>();
    pGameEye_->SetRotate({ 0.1f, 0.0f, 0.0f });
    pGameEye_->SetTranslate({ 0.0f, 0.2f, -5.0f });
    pGameEye_->SetName("MainCamera");

    /// システムにデフォルトのゲームカメラを設定
    Object3dSystem::GetInstance()->SetGlobalEye(pGameEye_.get());

    // ポストエフェクトの初期化
    this->_InitializePostEffects();

    pPEDissolve_->SetTextureResource(pTextureManager_->GetTextureResource("noise0.png"));

    // グリッドの初期化
    pModelGrid_ = std::make_unique<ObjModel>();
    pModelGrid_->Clone(pModelManager_->Load("Grid_v4/Grid_v4.obj"));
    pGrid_ = std::make_unique<Object3d>();
    pGrid_->Initialize();
    pGrid_->SetScale({ 1.0f, 1.0f, 1.0f });
    pGrid_->SetName("Grid");
    pGrid_->SetTilingMultiply({ 100.0f, 100.0f });
    pGrid_->SetEnableLighting(false);
    pGrid_->SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });
    pGrid_->SetModel(pModelGrid_.get());

    // 計測スタート
    globalTimer_.Start();
}

void CG5Task1::Finalize()
{
    pGrid_->Finalize();

    this->_FinalizePostEffects();
}

void CG5Task1::Update()
{
    pGameEye_->Update();
    pPERandomFilter_->SetSeed(globalTimer_.GetNow<float>() * 0.01f);

    pModelGrid_->Update();
    pGrid_->Update();
}

void CG5Task1::Draw()
{
    pGrid_->Draw();
}

void CG5Task1::DrawTexts()
{
}

void CG5Task1::_InitializePostEffects()
{
    pPEGrayscale_               = Helper::PostEffect::CreatePostEffect<Grayscale>(pDirectX_);
    pPEVignette_                = Helper::PostEffect::CreatePostEffect<Vignette>(pDirectX_);
    pPEBoxFilter_               = Helper::PostEffect::CreatePostEffect<BoxFilter>(pDirectX_);
    pPEGaussianFilter_          = Helper::PostEffect::CreatePostEffect<GaussianFilter>(pDirectX_);
    pPEPrewittOutline_          = Helper::PostEffect::CreatePostEffect<PrewittOutline>(pDirectX_);
    pPEDepthBasedOutline_       = Helper::PostEffect::CreatePostEffect<DepthBasedOutline>(pDirectX_);
    pPERadialBlur_              = Helper::PostEffect::CreatePostEffect<RadialBlur>(pDirectX_);
    pPEDissolve_                = Helper::PostEffect::CreatePostEffect<Dissolve>(pDirectX_);
    pPERandomFilter_            = Helper::PostEffect::CreatePostEffect<RandomFilter>(pDirectX_);
    pPESeparatedGaussianFilter_ = Helper::PostEffect::CreatePostEffect<SeparatedGaussianFilter>(pDirectX_);
    pPEGaussianBloom_           = Helper::PostEffect::CreatePostEffect<GaussianBloom>(pDirectX_);
    pPELuminanceOutput_         = Helper::PostEffect::CreatePostEffect<LuminanceOutput>(pDirectX_);

    (*pPostEffectExecuter_)
        .RegisterPostEffect(pPEGrayscale_.get())
        .RegisterPostEffect(pPEVignette_.get())
        .RegisterPostEffect(pPEBoxFilter_.get())
        .RegisterPostEffect(pPEGaussianFilter_.get())
        .RegisterPostEffect(pPEPrewittOutline_.get())
        .RegisterPostEffect(pPEDepthBasedOutline_.get())
        .RegisterPostEffect(pPERadialBlur_.get())
        .RegisterPostEffect(pPEDissolve_.get())
        .RegisterPostEffect(pPERandomFilter_.get())
        .RegisterPostEffect(pPESeparatedGaussianFilter_.get())
        .RegisterPostEffect(pPEGaussianBloom_.get())
        .RegisterPostEffect(pPELuminanceOutput_.get());
}

void CG5Task1::_FinalizePostEffects()
{
    (*pPostEffectExecuter_)
        .UnregisterPostEffect(pPEGrayscale_.get())
        .UnregisterPostEffect(pPEVignette_.get())
        .UnregisterPostEffect(pPEBoxFilter_.get())
        .UnregisterPostEffect(pPEGaussianFilter_.get())
        .UnregisterPostEffect(pPEPrewittOutline_.get())
        .UnregisterPostEffect(pPEDepthBasedOutline_.get())
        .UnregisterPostEffect(pPERadialBlur_.get())
        .UnregisterPostEffect(pPEDissolve_.get())
        .UnregisterPostEffect(pPERandomFilter_.get())
        .UnregisterPostEffect(pPESeparatedGaussianFilter_.get())
        .UnregisterPostEffect(pPEGaussianBloom_.get())
        .UnregisterPostEffect(pPELuminanceOutput_.get());
}

void CG5Task1::_DecomposeArg()
{
    try
    {
        pDirectX_ = std::any_cast<DirectX12*>(pArgs_->Get("DirectX12"));
        pPostEffectExecuter_ = std::any_cast<PostEffectExecuter*>(pArgs_->Get("PostEffectExecuter"));
        pModelManager_ = std::any_cast<ModelManager*>(pArgs_->Get("ModelManager"));
    }
    catch (const std::runtime_error& e)
    {
        pLogger_->LogError(__FILE__, __FUNCTION__, e.what());
        assert(0);
    }
}
