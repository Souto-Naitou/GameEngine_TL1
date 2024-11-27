#include "Color.h"
#include "Vector4.h"

Color::Color(const Vector4& _vec4)
{
    rgba.r = static_cast<unsigned int>(_vec4.x * 255.0f);
    rgba.g = static_cast<unsigned int>(_vec4.y * 255.0f);
    rgba.b = static_cast<unsigned int>(_vec4.z * 255.0f);
    rgba.a = static_cast<unsigned int>(_vec4.w * 255.0f);

    return;
}

Color::Color(unsigned int _color)
{
    rgba.r = (_color >> 24) & 0xFF;
    rgba.g = (_color >> 16) & 0xFF;
    rgba.b = (_color >> 8) & 0xFF;
    rgba.a = _color & 0xFF;

    return;
}

Vector4 Color::Vec4() const
{
    return {
        static_cast<float>(rgba.r) / 255.0f,
        static_cast<float>(rgba.g) / 255.0f,
        static_cast<float>(rgba.b) / 255.0f,
        static_cast<float>(rgba.a) / 255.0f
    };
}

unsigned int Color::UInt32() const
{
    unsigned int result = 0;
    result |= rgba.r << 24;
    result |= rgba.g << 16;
    result |= rgba.b << 8;
    result |= rgba.a;

    return result;
}
