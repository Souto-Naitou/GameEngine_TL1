#include "OBB.h"
#include "OBB.h"

void OBB::Initialize()
{
    pLineSystem = LineSystem::GetInstance();
    for (auto& line : lines)
    {
        line.Initialize();
        line.SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
    }
}

void OBB::Draw()
{
    Vector3 vertices[8]
    {
        {-this->size_.x, -this->size_.y, -this->size_.z}, // 0
        {-this->size_.x, this->size_.y, -this->size_.z},  // 1
        {this->size_.x, this->size_.y, -this->size_.z},   // 2
        {this->size_.x, -this->size_.y, -this->size_.z},  // 3
        {-this->size_.x, -this->size_.y, this->size_.z},  // 4
        {-this->size_.x, this->size_.y, this->size_.z},   // 5
        {this->size_.x, this->size_.y, this->size_.z},    // 6
        {this->size_.x, -this->size_.y, this->size_.z},   // 7
    };

    for (int i = 0; i < 8; i++)
    {
        vertices[i] = FMath::Transform(vertices[i], this->rotateMatrix_);
        vertices[i] = vertices[i] + this->center_;
    }

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

    for (auto& line : lines)
    {
        line.Update();
        line.Draw();
    }

    return;

}

void OBB::ModifyGameEye(GameEye* _eye)
{
    for (auto& line : lines)
    {
        line.SetGameEye(_eye);
    }
}
