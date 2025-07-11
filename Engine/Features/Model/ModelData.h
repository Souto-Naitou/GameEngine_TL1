#pragma once

#include <Matrix4x4.h>
#include <string>
#include <vector>
#include <Vector2.h>
#include <Vector4.h>
#include <Vector3.h>
#include <Math/Transform.h>

// contained in ModelData
struct Node
{
    QuaternionTransform transform = {};
    Matrix4x4 localMatrix = {};
    std::string name;
    std::vector<Node> children;
};

// contained in ModelData
struct MaterialData
{
    std::string textureFilePath;
    uint32_t padding[2] = {0, 0};
    Vector4 diffuse = {};
};

// contained in ModelData
struct VertexData
{
    Vector4 position = {};
    Vector2 texcoord = {};
    Vector3 normal = {};
};

struct ModelData
{
    std::vector<VertexData> vertices = {};
    std::vector<uint32_t> indices = {};
    MaterialData material = {};
    Node rootNode = {};
};