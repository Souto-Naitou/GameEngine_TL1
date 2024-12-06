#include <Framework/NimaFramework.h>
#include "SampleProgram/SampleProgram.h"


int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    NimaFramework* pGame = new SampleProgram();

    pGame->Run();

    delete pGame;

    return 0;
}