#include "fsutl.h"

#include <Utility/String/strutl.h>

std::filesystem::path utl::filesystem::to_lower(const std::filesystem::path& _path)
{
    std::filesystem::path result = {};

    result = string::to_lower(_path.string());

    return result;
}
