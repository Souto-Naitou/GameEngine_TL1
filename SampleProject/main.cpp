#include <Framework/NimaFramework.h>
#include "SampleProgram/SampleProgram.h"


int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    NimaFramework* pSampleProgram = new SampleProgram();

    pSampleProgram->Run();

    delete pSampleProgram;

    return 0;
}