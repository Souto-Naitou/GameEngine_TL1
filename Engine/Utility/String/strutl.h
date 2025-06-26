#pragma once

#include <string>
#include <cstdio>
#include <cstdint>

namespace utl
{
    namespace string
    {
        template <typename T>
        inline std::string to_string(const T* _ptr)
        {
            char buffer[20];
            std::snprintf(buffer, sizeof(buffer), "%p", static_cast<const void*>(_ptr));
            return std::string(buffer);
        }

        uint32_t to_hash(const std::string& _str);
    }
}