#include "Logger.h"
#include <Windows.h>

void Log(const std::string& _message)
{
	OutputDebugStringA(_message.c_str());
	return;
}