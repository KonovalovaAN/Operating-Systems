// Konovalova.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Konovalova.h"

extern "C" __declspec(dllexport) float add(float x, float y) {
    return x + y;
}

extern "C" __declspec(dllexport) float multiply(float x, float y) {
    return x * y;
}
// This is an example of an exported variable
KONOVALOVA_API int nKonovalova=0;

// This is an example of an exported function.
KONOVALOVA_API int fnKonovalova(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CKonovalova::CKonovalova()
{
    return;
}
