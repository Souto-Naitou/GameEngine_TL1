#include "OBB.h"
#include "OBB.h"

OBB::~OBB()
{
    delete lines;
}

void OBB::Initialize()
{
    pLineSystem = LineSystem::GetInstance();
    lines = new Line(12);
    lines->Initialize();
    lines->SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
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

    (*lines)[0] = vertices[0];
    (*lines)[1] = vertices[1];

    (*lines)[2] = vertices[1];
    (*lines)[3] = vertices[2];

    (*lines)[4] = vertices[2];
    (*lines)[5] = vertices[3];

    (*lines)[6] = vertices[3];
    (*lines)[7] = vertices[0];

    (*lines)[8] = vertices[0];
    (*lines)[9] = vertices[4];

    (*lines)[10] = vertices[4];
    (*lines)[11] = vertices[5];

    (*lines)[12] = vertices[5];
    (*lines)[13] = vertices[6];

    (*lines)[14] = vertices[6];
    (*lines)[15] = vertices[7];

    (*lines)[16] = vertices[7];
    (*lines)[17] = vertices[4];

    (*lines)[18] = vertices[1];
    (*lines)[19] = vertices[5];

    (*lines)[20] = vertices[2];
    (*lines)[21] = vertices[6];

    (*lines)[22] = vertices[3];
    (*lines)[23] = vertices[7];

    lines->Update();
    lines->Draw();

    return;

}

void OBB::ModifyGameEye(GameEye* _eye)
{
    lines->SetGameEye(_eye);
}
