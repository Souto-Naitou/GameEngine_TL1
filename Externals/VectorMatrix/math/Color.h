#pragma once

class Vector4;

/// <summary>
/// Color class
/// </summary>
class Color final
{
public:
    struct RGBA
    {
        unsigned int r : 8;
        unsigned int g : 8;
        unsigned int b : 8;
        unsigned int a : 8;
    };

    RGBA rgba;

    inline Color() : rgba() {};
    Color(const Vector4& _vec4);
    Color(unsigned int _color);


    Vector4         Vec4()      const;
    unsigned int    UInt32()    const;


};