#include "dbgutl.h"
#include <sstream>
#include <ios>

std::string utl::debug::generate_name_default(void* _ptr)
{
    std::stringstream ss;
    ss << "unnamed#0x" << std::hex << _ptr;
    return ss.str();
}
