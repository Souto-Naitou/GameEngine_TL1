#pragma once

#include <string>
#include <cstdio>

namespace utl
{
    namespace string
    {
        template <typename T>
        std::string to_string(const T* _ptr)
        {
            char buffer[20];
            std::snprintf(buffer, sizeof(buffer), "%p", static_cast<const void*>(_ptr));
            return std::string(buffer);
        }
    }
}