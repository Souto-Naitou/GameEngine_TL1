#pragma once

#include <string>
#include <cstdio>
#include <cstdint>

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

        uint32_t to_hash(const std::string& _str)
        {
            uint32_t hash = 0;
            for (char c : _str)
            {
                hash = (hash * 31) + static_cast<uint8_t>(c);
            }
            return hash;
        }
    }
}