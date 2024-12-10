#pragma once

#include <array>

#include <Vector3.h>
#include <Features/Line/Line.h>
#include <Features/Line/LineSystem.h>

class AABB
{
public:
    AABB() = default;

    void Initialize();

    void SetMinMax(const Vector3& _min, const Vector3& _max)
    {
        min_ = _min;
        max_ = _max;
    }

    void Draw();

private:
    Vector3 min_; // 最小点
    Vector3 max_; // 最大点

    std::array<Line, 12> lines;

private:
    LineSystem* pLineSystem;
};