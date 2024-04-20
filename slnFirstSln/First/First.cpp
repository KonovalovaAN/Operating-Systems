// First.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "..\ADLL\ADLL.h" // ==>"framework.h"==><windows.h> for HMODULE
#include <windows.h>
#include "..\BDLL\BDLL.h"
#include <TCHAR.h>

using namespace std;

//C:\Users\Riaby\source\repos\slnFirstSln\slnFirstSln.sln
// C:\Users\Riaby\source\repos\slnFirstSln\Debug
//C:\Users\Riaby\source\repos\slnFirstSln\ADLL
//C:\Users\Riaby\source\repos\slnFirstSln\First

#define BUFSIZE 500

extern __declspec(dllimport) int nx;

int main(int argc, char* argv[]) //main()
{
    //int exp = ADLL_EXPORTS; //Error
    nx++;
    printf("nx:%d\n\n", nx);
    
    std::cout << "Hello World!\n\n";
    std::cout << "nBDLL is " << nBDLL << endl;
    std::cout << "fnBDLL is " << fnBDLL() << endl;

    TCHAR buffer[BUFSIZE] = { 0, };
    DWORD dret;

    dret = GetModuleFileName(NULL, buffer, BUFSIZE);//1
    _tprintf(TEXT("Executable Path  (%s)\n\n"), buffer);


     dret = GetSystemDirectory(buffer,BUFSIZE);//2
    _tprintf(TEXT("System Directory (%s)\n\n"), buffer);

    dret = GetWindowsDirectory(buffer, BUFSIZE);//3
    _tprintf(TEXT("Windows Directory (%s)\n\n"), buffer);

    dret = GetCurrentDirectory(BUFSIZE, buffer);//4
    _tprintf(TEXT("Current Directory (%s)\n\n"), buffer);

    
    
    //   
    //PATH //5 
    //If SafeDllSearchMode is disabled, the search order is as follows:
    // 1,4,2,3,5
    // 
    dret = GetModuleFileName(hModuleADLL, buffer, BUFSIZE);//
    _tprintf(TEXT("ADLL Path  (%s)\n\n"), buffer);
    printf("hModuleADLL is the base :%08x\n\n", hModuleADLL);

    dret = GetModuleFileName(hModuleBDLL, buffer, BUFSIZE);//
    _tprintf(TEXT("BDLL Path  (%s)\n\n"), buffer);
    printf("hModuleBDLL is the base :%08x\n\n", hModuleBDLL);

    HMODULE hModuleKernel32=GetModuleHandle(TEXT("kernel32.dll"));
    printf("hModuleKernel32 is the base :%x\n\n", hModuleKernel32);

    HMODULE hModuleFirst = GetModuleHandle(NULL);//TEXT("First.exe")
    printf("hModuleFirst is the base :%08x\n\n", hModuleFirst);

    //goto l1;
    printf("argc:%d\n",argc);
    printf("argv[0]:%s\n", argv[0]);
    //
    for (int i = 1; i < argc; i++)
        printf("argv[%d]:%s\n",i, argv[i]);
    //
    printf("\n");
    printf("Parameters as Integer\n\n");
    for (int i = 1; i < argc; i++)
    {
        int iargv = atoi(argv[i]);
        printf("argv[%d]:%d\n", i, iargv);
    }
    printf("Press any Key to run:\n");
    getchar();

//l1:
   // printf("Call imported functions and variables\n");
    printf("nADLL:%d\n\n", nADLL);   

    printf("fnADLL:%d\n\n", fnADLL());

    printf("Press any Key to run:\n");
    getchar();

    //5)Enumeration: GetModuleFileName

    _TCHAR szFileName[4000] = { 0, };
    DWORD dwRet;

    for (unsigned h = 0x10000; h < 0x80000000; h += 0x10000)
    {
        dwRet = GetModuleFileName((HMODULE)h, szFileName, 4000);
        if (dwRet)
            _tprintf(L"\nAt Base:%08x Module File :%s\n", h, szFileName);
    }
    printf("Press any Key to quit:\n");
    //getchar();
    cin.get();
    
    return(0);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
