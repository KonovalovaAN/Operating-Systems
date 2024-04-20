// FileToFile.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
//#include "resource.h"  //COMMON
#include <string>
//#include "resource.h"  //COMMON
//#include "WndHandles.h"
#include <stdlib.h>
#include <iostream>


bool CopyFileToFile(HANDLE hSourceFile, HANDLE hDestFile) {

    TCHAR  inBuf[512];
    DWORD  nBytesRead, nBytesWritten;
    BOOL   bResult;

    while (true)
    {
        bResult = ReadFile(hSourceFile, inBuf, 512, &nBytesRead, NULL);//Offset not used
        if (bResult && nBytesRead == 0)
            break;//End of File
        WriteFile(hDestFile, inBuf, nBytesRead, &nBytesWritten, NULL);
    }
    return true;
}//CopyFileToFile


bool Simplest(LPSTR szKey, HANDLE hDestFile)
{
    DWORD dwFileSize;
    HANDLE hFileMapping;
    LPVOID lpFileMap;
    SYSTEM_INFO  SystemInfo;

    //---Get a File Size of  hDestFile

    dwFileSize = GetFileSize(hDestFile,
        NULL);// pointer to high-order word for file size
    //--Get System Allocation  Granularity
    GetSystemInfo(&SystemInfo);  // address of system information structure

    TCHAR szGranularity[400];
    wsprintf(szGranularity, "Gr=%ld Pg=%ld", SystemInfo.dwAllocationGranularity, SystemInfo.dwPageSize);
    MessageBox(NULL, szGranularity, "Granularity", MB_OK);

    //----Specify "view window"-------------------------------------------//

    DWORD dwViewWindowSize = SystemInfo.dwAllocationGranularity;//or some times of that

    //----Specify how many "view windows" cover the file hDestFile--------//

    DWORD dwNumberOfWindows = dwFileSize / dwViewWindowSize;
    DWORD dwResidue = dwFileSize % dwViewWindowSize;

    unsigned k, i;

    //------The whole file is to be processed--------//
    DWORD dwMaximumSizeLow = dwFileSize; //
    //-----------------------------------------------//
    hFileMapping = CreateFileMapping(hDestFile,
        NULL,
        //PAGE_WRITECOPY,
        PAGE_READWRITE,  //The file specified by hFile must have been created
        //with GENERIC_READ and GENERIC_WRITE access.
        0,
        dwMaximumSizeLow,  //Specifies the low-order 32 bits of the maximum size 
        //of the file-mapping object.
    //If this parameter and dwMaximumSizeHigh are zero,
    // the maximum size of the file-mapping object is equal to 
    //the current size of the file identified by hFile
        NULL);      //the name of the mapping object. 

    if (hFileMapping == NULL)    return false;



    //----------Loop on dwNumberOfWindows---------------------------------//
    for (i = 0; i < dwNumberOfWindows; i++)
    {


        lpFileMap = MapViewOfFile(
            hFileMapping,
            //FILE_MAP_COPY,
            FILE_MAP_WRITE, //The hFileMappingObject parameter 
            //must have been created with PAGE_READWRITE protection. 
            //A read-write view of the file is mapped.
            0,
            (DWORD)(i * dwViewWindowSize),  //an offset within the file
            dwViewWindowSize);        //Specifies the number of bytes of the file to map.
        //If dwNumberOfBytesToMap is zero, the entire file is mapped. 
        if (NULL == lpFileMap) {
            TCHAR mess[200];
            wsprintf(mess, "NULL == lpFileMap Error:%ld", GetLastError());
            MessageBox(NULL, mess, "1)MapViewOfFile", MB_OK);
            CloseHandle(hFileMapping);
            return false;
        }

        //|Encript

        //---------Encript "view window" under a number i ------------//   
        for (k = 0; k < dwViewWindowSize; k++)
            ((char*)lpFileMap)[k] = ((char*)lpFileMap)[k] ^ szKey[0];

        //------All or some "view windows" can be flushed-------------//
        //    FlushViewOfFile(  lpFileMap,  // start address of byte range to flush
        //            dwViewWindowSize );// number of bytes in range 


        UnmapViewOfFile(lpFileMap);


    }//--continue for-loop

    //------Encript a remainder of the file hDestFile-----//

    if (!dwResidue) {
        CloseHandle(hFileMapping);  return true;
    }



    lpFileMap = MapViewOfFile(hFileMapping,

        //FILE_MAP_COPY,
        FILE_MAP_WRITE, //The hFileMappingObject parameter 
        //must have been created with PAGE_READWRITE protection. 
        //A read-write view of the file is mapped.
        0,
        dwFileSize - dwResidue,  //an offset within the file
        dwResidue);        //Specifies the number of bytes of the file to map.

    if (NULL == lpFileMap) {
        TCHAR mess[200];
        wsprintf(mess, "NULL == lpFileMap Error:%ld", GetLastError());
        MessageBox(NULL, mess, "2)MapViewOfFile", MB_OK);
        CloseHandle(hFileMapping);
        return false;
    }
    //-----Encript the remainder of the file hDestFile-

    for (k = 0; k < dwResidue; k++) ((char*)lpFileMap)[k] = ((char*)lpFileMap)[k] ^ szKey[0];

    UnmapViewOfFile(lpFileMap);

    CloseHandle(hFileMapping);  return true;

}//Simplest

BOOL EncriptFileToFile(LPSTR lpstrSourceFile, LPSTR lpstrDestinationFile, LPSTR key)
{
    printf("Source: %s\nDestination: %s\nDirection: %s\nKey: %s\n",
        lpstrSourceFile,
        lpstrDestinationFile, "Backward", key);

    //-----------------------------------------
    //Create Handles to Source and Destination Files 
    //-----------------------------------------
    HANDLE hSourceFile, hDestFile;

    //Create Handle to Source File 
    hSourceFile = CreateFile(
        lpstrSourceFile,        // pointer to name of the file
        GENERIC_READ,      // access (read-write) mode
        FILE_SHARE_READ,        // share mode
        NULL,                   // pointer to security attributes
        OPEN_EXISTING,          // how to create
        FILE_ATTRIBUTE_READONLY | FILE_FLAG_SEQUENTIAL_SCAN,
        //|FILE_FLAG_OVERLAPPED,   // file attributes
        NULL                    // handle to file with attributes to copy
    );

    //hSourceFile has just created!
    //--------------------------------------------
    //Create Handle to Destination File 
    hDestFile = CreateFile(
        lpstrDestinationFile, // pointer to name of the file
        GENERIC_WRITE,      // access (read-write) mode
        0,            // If dwShareMode is 0, the object cannot be shared. 
        //Subsequent open operations on the object will fail,
        //until the handle is closed. 
        NULL,                   // pointer to security attributes
        CREATE_ALWAYS,          // how to create
        FILE_ATTRIBUTE_NORMAL,
        //|FILE_FLAG_OVERLAPPED,   // file attributes
        NULL                    // handle to file with attributes to copy
    );


    bool res;

    res = CopyFileToFile(hSourceFile, hDestFile);

    if (!res)
    {
        MessageBox(NULL, TEXT("CopyFileToFile failed... "), TEXT("Encript File To File"), MB_OK);
        CloseHandle(hSourceFile);
        CloseHandle(hDestFile);
        return false;
    }
    CloseHandle(hSourceFile);
    CloseHandle(hDestFile);

    //##################################################//
    //Create Handle to Destination File to be mapped onto memory

    hDestFile = CreateFile(
        lpstrDestinationFile,        // pointer to name of the file
        GENERIC_READ | GENERIC_WRITE,    // access (read-write) mode
        0,            // If dwShareMode is 0, the object cannot be shared. 
        //Subsequent open operations on the object will fail,
        //until the handle is closed. 
        NULL,                   // pointer to security attributes
        OPEN_EXISTING,          // how to create
        0,            // file attributes
        NULL                    // handle to file with attributes to copy
    );

    //2)hDestFile has just created!
    //--------------------------------------------   
    res = Simplest(key, hDestFile);

    CloseHandle(hDestFile);
    return res;

}/////EncriptFileToFile

int main(int argc, char** argv) {
    std::string srcFile = argv[1] + '\0'; 
    std::string destFile = argv[2] + '\0';
    std::string key;
    std::cout << "Key: ";
    std::cin >> key;
    key += '\0';
    EncriptFileToFile(&srcFile[0], &destFile[0], &key[0]);
}

