#include "AABB.h"

void AABB::Initialize()
{
    pLineSystem = LineSystem::GetInstance();
    for (auto& line : lines)
    {
        line.Initialize();
    }
}

void AABB::Draw()
{
    Vector3 vertices[8]
    {
        {min_.x, min_.y, min_.z}, // 0
        {min_.x, max_.y, min_.z}, // 1
        {max_.x, max_.y, min_.z}, // 2
        {max_.x, min_.y, min_.z}, // 3
        {min_.x, min_.y, max_.z}, // 4
        {min_.x, max_.y, max_.z}, // 5
        {max_.x, max_.y, max_.z}, // 6
        {max_.x, min_.y, max_.z}, // 7
    };

    lines[0].GetVertices()[0] = vertices[0];
    lines[0].GetVertices()[1] = vertices[1];

    lines[1].GetVertices()[0] = vertices[1];
    lines[1].GetVertices()[1] = vertices[2];

    lines[2].GetVertices()[0] = vertices[2];
    lines[2].GetVertices()[1] = vertices[3];

    lines[3].GetVertices()[0] = vertices[3];
    lines[3].GetVertices()[1] = vertices[0];

    lines[4].GetVertices()[0] = vertices[0];
    lines[4].GetVertices()[1] = vertices[4];

    lines[5].GetVertices()[0] = vertices[4];
    lines[5].GetVertices()[1] = vertices[5];

    lines[6].GetVertices()[0] = vertices[5];
    lines[6].GetVertices()[1] = vertices[6];

    lines[7].GetVertices()[0] = vertices[6];
    lines[7].GetVertices()[1] = vertices[7];

    lines[8].GetVertices()[0] = vertices[7];
    lines[8].GetVertices()[1] = vertices[4];

    lines[9].GetVertices()[0] = vertices[1];
    lines[9].GetVertices()[1] = vertices[5];

    lines[10].GetVertices()[0] = vertices[2];
    lines[10].GetVertices()[1] = vertices[6];

    lines[11].GetVertices()[0] = vertices[3];
    lines[11].GetVertices()[1] = vertices[7];

    pLineSystem->PresentDraw();
    for (auto& line : lines)
    {
        line.Update();
        line.Draw();
    }

    return;
}
