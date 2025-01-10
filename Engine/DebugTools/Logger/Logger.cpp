#include "Logger.h"

#include <debugapi.h>

void Log(const std::string& _message)
{
	OutputDebugStringA(_message.c_str());
	return;
}