#pragma once
#include <filesystem>

namespace utl::filesystem
{
    std::filesystem::path to_lower(const std::filesystem::path& _path);
}