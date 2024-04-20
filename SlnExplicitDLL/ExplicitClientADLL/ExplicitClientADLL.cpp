// ExplicitClientADLL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\\ADLL\\Interface.h"
#include <windows.h>
#include <iostream>
//See "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	pMyInt pN;
	pMyFunc pFn;
	pPow pPw;
	pCreateObject pCrObj;
	std::cout << "Starting..." << std::endl;

	HMODULE hExe = GetModuleHandle(NULL);
	std::wcout << "hExe is Base:" << std::endl << hExe << std::endl;

//	HMODULE hLib = LoadLibraryEx(TEXT("D:\\OS2223Z\\19_09\\SlnExplicitDLL\\debug\\ADLL.dll"),
	//	NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	//	HMODULE hLib = LoadLibraryEx(TEXT("ADLL"),
	//	NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	//HMODULE hLib=LoadLibraryA("ADLL");
	//HMODULE hLib = LoadLibrary(L"ADLL");
	HMODULE hLib = LoadLibrary(TEXT("ADLL"));
	if (!hLib)
	{
		std::cout << "Error=" << GetLastError() << std::endl;
		std::cin.get();
		return 1;
	}
	std::wcout << "hLib is Base:" << std::endl << hLib << std::endl;
	_TCHAR  wbuffer [MAX_PATH];
	GetModuleFileName(hLib, wbuffer, MAX_PATH);
	std::wcout << "ADLL path:" << std::endl << wbuffer << std::endl;

	//pN = (pMyInt)GetProcAddress(hLib,"?nADLL@@3HA");
	pN = (pMyInt)GetProcAddress(hLib,"nADLL");
	if (pN)
		std::cout << "pN=" << *pN << std::endl;
	else
		std::cout << "Error variable!" << std::endl;

	pFn = (pMyFunc)GetProcAddress(hLib,"fnADLL");
	if (pFn)
		std::cout << "pFn=" << pFn() << std::endl;
	else
		std::cout << "Error Function Call!" << std::endl;

	pPw = (pPow)GetProcAddress(hLib, "Pow");
	if (pPw) 
		std::cout << "pFw=" << pPw(3, 3) << std::endl;
	else
		std::cout << "Error Function Call!" << std::endl;

//	pCreateObject pCrObj;

	pCrObj = (pCreateObject)GetProcAddress(hLib,"CreateMyObject");
	if (pCrObj)
	{
		IMyObject* pIMyObject;
		double d;
		int iRet = pCrObj(pIMyObject);
		std::cout << "CreateObject Return Value!=" << iRet << std::endl;
		std::cout << "CreateObject Parameter Value!=" << pIMyObject << std::endl;

//		if (iRet)
//		{
			pIMyObject->WriteProperty(13.5);
			pIMyObject->ReadProperty(d);
			std::cout << "d =" << d << std::endl;
			double y = pIMyObject->MySin(3.14);
			std::cout << "y =" << y << std::endl;
			//pIMyObject->Destroy();   //First Case
			FreeLibrary(hLib);         //Second Case

//		}
//		else
//			std::cout << "Error Function CreateObject Return Value!" << std::endl;

	}
	else
		std::cout << "Error Function CreateObject Call!" << std::endl;
	std::cout << "Finishing..." << std::endl;
	std::cin.get();
	return 0;
}

