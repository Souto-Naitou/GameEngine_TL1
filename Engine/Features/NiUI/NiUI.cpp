#include "NiUI.h"

#include <stdexcept> // runtime_error

NiUI_Input NiUI::input_ = NiUI_Input();
bool NiUI::isInitialized_ = false;
bool NiUI::isBeginFrame_ = false;
NiVec2 NiUI::leftTop_ = { 0, 0 };
NiVec2 NiUI::size_ = { 0, 0 };
std::unordered_map<std::string, ButtonImageData> NiUI::buttonImages_ = std::unordered_map<std::string, ButtonImageData>();
std::string NiUI::activeComponentID_ = {};
IDrawer* NiUI::drawer_ = nullptr;


void NiUI::Initialize(const NiVec2& _size, const NiVec2& _leftTop)
{
    isInitialized_ = true;
    leftTop_ = _leftTop;
    size_ = _size;

    input_.Initialize();

    return;
}


void NiUI::BeginFrame()
{
    CheckValid_BeginFrame();

    // 入力データ更新
    input_.Update();

    // 確認用フラグを立てる
    isBeginFrame_ = true;

    return;
}

void NiUI::DrawUI()
{
    // 描画処理に必要なデータの確認
    CheckValid_DrawUI();

    // ボタンの描画データを追加
    ButtonDataEnqueue();

    // 描画前処理
    drawer_->DrawSetting();

    // 描画
    drawer_->Draw();

    // 確認用フラグを倒す
    isBeginFrame_ = false;
}

void NiUI::NiUI_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    input_.WndProcHandler(hWnd, msg, wParam, lParam);
    return;
}

bool NiUI::Button(const std::string& _id, const std::string& _textureName, const NiVec2& _leftTop, const NiVec2& _size)
{
    auto& buttonImage = buttonImages_[_id];
    bool isTrigger = false;
    bool isHover = false;
    bool isRelease = false;
    bool isHeld = false;

    // 当たり判定
    JudgeClickRect(_leftTop, _size, isHover, isTrigger, isRelease);

    // ボタンの挙動
    bool result = ButtonBehavior(_id, isHover, isTrigger, isRelease, isHeld);

    /// ボタンのデータを更新
    buttonImage.textureName = _textureName;
    buttonImage.leftTop = _leftTop;
    buttonImage.size = _size;
    buttonImage.isHover = isHover;
    buttonImage.isHeld = isHeld;

    return result;
}

bool NiUI::ButtonBehavior(const std::string& _id, bool _isHover, bool _isTrigger, bool _isRelease, bool& _out_held)
{
    if(_isTrigger)
    {
        activeComponentID_ = _id;
    }

    if(activeComponentID_ == _id)
    {
        _out_held = true;

        if(_isRelease)
        {
            activeComponentID_ = {};
            return true;
        }
    }

    return false;
}

void NiUI::ButtonDataEnqueue()
{
    for(auto& buttonImage : buttonImages_)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&buttonImage.second);
    }
}

void NiUI::CheckValid_BeginFrame()
{
    if(!isInitialized_)
    {
        throw std::runtime_error("UIクラスが初期化されていません。");
    }

    if(isBeginFrame_)
    {
        throw std::runtime_error("BeginFrameが連続で呼び出されています。");
    }
}

void NiUI::CheckValid_DrawUI()
{
    if(!isInitialized_)
    {
        throw std::runtime_error("UIクラスが初期化されていません。");
    }

    if(!isBeginFrame_)
    {
        throw std::runtime_error("BeginFrameが呼び出されていません。");
    }

    if(drawer_ == nullptr)
    {
        throw std::runtime_error("Drawerがセットされていません。");
    }
}

void NiUI::JudgeClickRect(const NiVec2& _leftTop, const NiVec2& _size, bool& _isHover, bool& _isTrigger, bool& _isRelease)
{
    NiVec2 leftTop = _leftTop + leftTop_;

    if(leftTop.x <= input_.GetMousePos().x && input_.GetMousePos().x <= leftTop.x + _size.x &&
        leftTop.y <= input_.GetMousePos().y && input_.GetMousePos().y <= leftTop.y + _size.y)
    {
        _isHover = true;
        if(input_.TriggerLeft())
        {
            _isTrigger = true;
        }
        else
        {
            _isTrigger = false;
        }
    }
    else
    {
        _isHover = false;
        _isTrigger = false;
    }

    if(input_.ReleaseLeft())
    {
        _isRelease = true;
    }
    else
    {
        _isRelease = false;
    }
}
