#pragma once

#include <Vector3.h>
#include <Vector4.h>

#include <nlohmann/json.hpp>

void from_json(const nlohmann::json& _j, Vector3& _v)
{
    _j.at("x").get_to(_v.x);
    _j.at("y").get_to(_v.y);
    _j.at("z").get_to(_v.z);
}

void to_json(nlohmann::json& _j, const Vector3& _v)
{
    _j = nlohmann::json{ {"x", _v.x}, {"y", _v.y}, {"z", _v.z} };
}

void from_json(const nlohmann::json& _j, Vector4& _v)
{
    _j.at("x").get_to(_v.x);
    _j.at("y").get_to(_v.y);
    _j.at("z").get_to(_v.z);
    _j.at("w").get_to(_v.w);
}

void to_json(nlohmann::json& _j, const Vector4& _v)
{
    _j = nlohmann::json{ {"x", _v.x}, {"y", _v.y}, {"z", _v.z}, {"w", _v.w} };
}