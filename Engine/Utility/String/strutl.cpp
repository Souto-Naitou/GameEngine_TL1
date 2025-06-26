#include "strutl.h"

uint32_t utl::string::to_hash(const std::string& _str)
{
    uint32_t hash = 0;
    for (char c : _str)
    {
        hash = (hash * 31) + static_cast<uint8_t>(c);
    }
    return hash;
}
