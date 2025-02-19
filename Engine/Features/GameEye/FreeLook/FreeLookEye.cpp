#include "FreeLookEye.h"

FreeLookEye::FreeLookEye()
{
    pInput_ = Input::GetInstance();
}

FreeLookEye::~FreeLookEye()
{
}

void FreeLookEye::Update()
{
    prevCursorPos_ = currCursorPos_;
    GetCursorPos(&currCursorPos_);

    if(pInput_->PushMouse(Input::MouseNum::Right))
    {
        this->CatchMoveCommands();
        this->CatchRotateCommands();
    }

    GameEye::Update();
}

void FreeLookEye::CatchMoveCommands()
{
    Vector3 rotate = GetTransform().rotate;
    Quaternion yaw = Quaternion::RotateAxisAngleQuaternion({ 0.0f, 1.0f, 0.0f }, rotate.y);
    Quaternion pitch = Quaternion::RotateAxisAngleQuaternion({ 1.0f, 0.0f, 0.0f }, rotate.x);

    rotate_ = yaw * pitch;

    Vector3 forward = FMath::RotateVector({ 0.0f, 0.0f, 1.0f }, rotate_);
    Vector3 right = FMath::RotateVector({ 1.0f, 0.0f, 0.0f }, rotate_);

    /// 向いている方向に移動
    if(pInput_->PushKeyC('W'))
    {
        SetTranslate(GetTransform().translate + forward * moveSpeed_);
    }
    if(pInput_->PushKeyC('S'))
    {
        SetTranslate(GetTransform().translate - forward * moveSpeed_);
    }
    if(pInput_->PushKeyC('A'))
    {
        SetTranslate(GetTransform().translate - right * moveSpeed_);
    }
    if(pInput_->PushKeyC('D'))
    {
        SetTranslate(GetTransform().translate + right * moveSpeed_);
    }
    if(pInput_->PushKeyC('Q'))
    {
        SetTranslate(GetTransform().translate + Vector3(0.0f, -moveSpeed_, 0.0f));
    }
    if(pInput_->PushKeyC('E'))
    {
        SetTranslate(GetTransform().translate + Vector3(0.0f, moveSpeed_, 0.0f));
    }
}

void FreeLookEye::CatchRotateCommands()
{
    Vector2 diff = { 
        static_cast<float>(currCursorPos_.x - prevCursorPos_.x), 
        static_cast<float>(currCursorPos_.y - prevCursorPos_.y) 
    };

    Transform transform = GetTransform();
    transform.rotate.x += diff.y * 0.003f;
    transform.rotate.y += diff.x * 0.003f;

    SetTransform(transform);
}
