#include "UI.h"

#include <stdexcept> // runtime_error

UI_Input UI::input_ = UI_Input();
bool UI::isInitialized_ = false;
bool UI::isBeginFrame_ = false;
NiVec2 UI::leftTop_ = { 0, 0 };
NiVec2 UI::size_ = { 0, 0 };
std::unordered_map<std::string, ButtonImageData> UI::buttonImages_ = std::unordered_map<std::string, ButtonImageData>();
std::string UI::activeComponentID_ = {};
IDrawer* UI::drawer_ = nullptr;


void UI::Initialize(const NiVec2& _size, const NiVec2& _leftTop)
{
    isInitialized_ = true;
    leftTop_ = _leftTop;
    size_ = _size;

    input_.Initialize();

    return;
}


void UI::BeginFrame()
{
    CheckValid_BeginFrame();

    // 入力データ更新
    input_.Update();

    // 確認用フラグを立てる
    isBeginFrame_ = true;

    return;
}

void UI::DrawUI()
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

void UI::NiUI_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    input_.WndProcHandler(hWnd, msg, wParam, lParam);
    return;
}

bool UI::Button(const std::string& _id, const std::string& _textureName, const NiVec2& _leftTop, const NiVec2& _size)
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

bool UI::ButtonBehavior(const std::string& _id, bool _isHover, bool _isTrigger, bool _isRelease, bool& _out_held)
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

void UI::ButtonDataEnqueue()
{
    for(auto& buttonImage : buttonImages_)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&buttonImage.second);
    }
}

void UI::CheckValid_BeginFrame()
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

void UI::CheckValid_DrawUI()
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

void UI::JudgeClickRect(const NiVec2& _leftTop, const NiVec2& _size, bool& _isHover, bool& _isTrigger, bool& _isRelease)
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
