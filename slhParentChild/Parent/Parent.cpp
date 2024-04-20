// Parent.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Windows.h"


//int main()
int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";

    //A) Create the file in the Current Directory of the process "Parent.exe"

    char fileName[] = "ResultFromChild.txt";
    
    SECURITY_ATTRIBUTES saFile = { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };

    HANDLE hFile = CreateFileA(fileName,          // name of the write
             GENERIC_READ|GENERIC_WRITE,          // open for reading and writing
            FILE_SHARE_READ|FILE_SHARE_WRITE,     // share
            &saFile,                              // bInheritHandle==TRUE
            CREATE_ALWAYS,             // create new file or rewrite
            FILE_ATTRIBUTE_NORMAL,     // normal file
            NULL);                     // no attr. template

    if (hFile == INVALID_HANDLE_VALUE)    {        
        printf("CreateFile: Unable to open file \"%s\" for write.\n", fileName);
        return 1;
    }
    //1) Make szQuery for "Child.exe"

    char szQuery[256] = { '\0', };
    sprintf_s(szQuery, 256, "%s %7.4f\n", "square", -5.0);
    //szQuery == "square   0.625"  

    //2) Write szQuery into the file as the kernel object hFile
    //
    
    //DWORD lenQuery = strcspn(szQuery,"\0");
    DWORD dwBytesWritten = 0; 
    DWORD sizeSqrt = sizeof("square")-1;
    DWORD lenQuery = (sizeSqrt + 1 + 7 + 1);//Excluding '\0' at the end of "szQuery"

   bool bErrorFlag = WriteFile(
        hFile,           // open file handle
        szQuery,        // start of data to write
        lenQuery,       // number of bytes to write
        &dwBytesWritten, // number of bytes that were written
        NULL);            // no overlapped structure

    if (FALSE == bErrorFlag)    {        
        printf("Unable to write to file \"%s\".\n", fileName);
        CloseHandle(hFile);
        return 2;
    }
    else
    {
        if (dwBytesWritten != lenQuery)
        {
            // This is an error because a synchronous write that results in
            // success (WriteFile returns TRUE) should write all data as
            // requested. This would not necessarily be the case for
            // asynchronous writes.
            printf("Error: dwBytesWritten != lenQuery\n");
            CloseHandle(hFile);
            return 2;
        }
        else
        {
            printf("Wrote %d bytes to %s successfully.\n", dwBytesWritten, fileName);
        }
    }

    printf("\nhFile: %d ; %#10x\n", (DWORD)hFile, hFile);
    printf("\nQuery:\n%s\n", szQuery);

    //////////////////////////////////////////////////////////////////////////////////
    //B) The parent process "Parent.exe" is creating the child's process "Child.exe" 

    //1) Set the Command Line for the child's process
    //
    char szCommandLine[256] = { '\0', };
    sprintf_s(szCommandLine, 256, "%s %d", "Child.exe", (DWORD)hFile);    

    //2) Set dwFlags
    //
    DWORD dwFlags = CREATE_NEW_CONSOLE | BELOW_NORMAL_PRIORITY_CLASS; //for no /B and for /BELOWNORMAL
       
    //3) Set the Current Directory for the child's process
    //
    char* pszCurrentDirectory = NULL; //If NULL the Current Directory of "Parent.exe" will be!
    
    //4) Set the startup information for the child's process
    //
    STARTUPINFOA si = { sizeof(STARTUPINFOA) };

    char childTitle[] = "Child.exe";
    si.lpTitle = childTitle;
    /* si.dwFlags = STARTF_USESHOWWINDOW;
     si.wShowWindow = SW_SHOWMAXIMIZED;*/

    //5)Prepare the structure "pi" for saving "handles" and "identifiers" of two types of "kernel objects"
    //"process" and primary "thread" created by "Parent.exe" for "Child.exe"
    //
    /*   typedef struct _PROCESS_INFORMATION {
        HANDLE hProcess;
        HANDLE hThread;
        DWORD dwProcessId;
        DWORD dwThreadId;
    } PROCESS_INFORMATION, * PPROCESS_INFORMATION, * LPPROCESS_INFORMATION;
    */

    PROCESS_INFORMATION pi;

    //6)Prepare SECURITY_ATTRIBUTES for two types of "kernel objects" 
    //created by "Parent.exe" for "Child.exe"

    //typedef struct _SECURITY_ATTRIBUTES {
    //    DWORD nLength;
    //    LPVOID lpSecurityDescriptor;
    //    BOOL bInheritHandle;
    //} SECURITY_ATTRIBUTES, * PSECURITY_ATTRIBUTES, * LPSECURITY_ATTRIBUTES;

    SECURITY_ATTRIBUTES saChProcess       = { sizeof(SECURITY_ATTRIBUTES),NULL,FALSE };
    SECURITY_ATTRIBUTES saChPrimaryThread = { sizeof(SECURITY_ATTRIBUTES),NULL,FALSE };

    BOOL  fSuccess;
    //goto lexit;
    fSuccess = CreateProcessA(
        NULL,           //Always
        szCommandLine, //For a Child Process
        NULL, //SECURITY_ATTRIBUTES for Child Process
        NULL, //SECURITY_ATTRIBUTES for Primary Thread of Child Process
        TRUE,  //Child Process "inherits parent's kernel objects" (bInheritHandles==TRUE): hFile 
        dwFlags,
        NULL,     //Child Process "inherits parent's environment"  
        pszCurrentDirectory,//For Child Process. If NULL then it inherits the Current Directory of the parent process!
        &si,
        &pi);
    if (!fSuccess)
    {
        printf("CreateProcess failed (%d)\n", GetLastError());
        printf("\nPress any Key to quit:\n");
        std::cin.get();
        return 1;
    }
    //4)Wait for ending Child Process until it executes Query and returns the result in the file

    WaitForSingleObject(pi.hProcess, INFINITE); //pi.hProcess is the HANDLE of Child Process

    //////////////////////////////////////////////////////////////////////////
    printf("\nChild Process with PID:%d Ended...\n", pi.dwProcessId);
    printf("\nPrimary Thread of Child Process TID:%d Ended...\n", pi.dwThreadId);

//lexit:
    //5) Read all the file as an answer to Query from Child Process
    //
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    DWORD fileSize = GetFileSize(hFile, NULL);
    DWORD bytesRead = 0;
    char answerToQuery[500] = {'\0',};
    //
    bool bReadFile = ReadFile(hFile, answerToQuery,fileSize, &bytesRead,NULL);
    
    printf("\nAnswer To Query:\n%s\n", answerToQuery);
    CloseHandle(hFile);


    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("\nPress any Key to quit:\n");
    std::cin.get();
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
