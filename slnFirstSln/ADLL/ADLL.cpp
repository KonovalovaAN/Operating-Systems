// ADLL.cpp : Defines the exported functions for the DLL.
//

//#include "framework.h"
#include "ADLL.h"


// This is an example of an exported variable
ADLL_API int nADLL=123;
//
ADLL_API HMODULE hModuleADLL = 0;

// This is an example of an exported function.
ADLL_API int fnADLL(void)
{
    return 2+ADLL_EXPORTS;
}

// This is the constructor of a class that has been exported.
CADLL::CADLL()
{
    return;
}
////
extern __declspec(dllexport) int nx = 2022;
