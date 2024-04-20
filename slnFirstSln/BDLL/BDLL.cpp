// BDLL.cpp : Определяет экспортируемые функции для DLL.
//

#include "pch.h"
#include "framework.h"
#include "BDLL.h"


// Пример экспортированной переменной
BDLL_API int nBDLL=2023;
BDLL_API HMODULE hModuleBDLL = 0;

// Пример экспортированной функции.
BDLL_API int fnBDLL(void)
{
    return 2019;
}

// Конструктор для экспортированного класса.
CBDLL::CBDLL()
{
    return;
}
