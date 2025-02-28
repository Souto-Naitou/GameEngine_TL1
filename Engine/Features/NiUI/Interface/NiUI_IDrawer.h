#pragma once

#include "../Type/ComponentData.h"
#include <list>

class IDrawer
{
public:
    IDrawer() = default;
    virtual ~IDrawer() = default;

    virtual void EnqueueDrawInfo(const ButtonImageData* _data) { buttonDrawDataQueue_.push_back(_data); }
    virtual void DrawSetting();
    virtual void Draw() = 0;

protected:
    std::list<const ButtonImageData*> buttonDrawDataQueue_;
};