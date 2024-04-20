// Child.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Windows.h"


//int main()
int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
    if (argc != 2)
    {
        printf("\nError: argc:%d != 2 \n", argc);
        return 1;
    }
    //DWORD 
        HANDLE    dwHandle = 0;

    //1) Get the handle "hFile" inherited from "Parent.exe"
    //
    sscanf_s(argv[1], "%u", &dwHandle);
   
    HANDLE hFile = (HANDLE)dwHandle;

    //std::cout << "Press Key Enter to run\n";
    //std::cin.get();
    //return 0;
    // 
    //2) Get "fileSize" (The kernel object hFile has been opened by "Parent.exe" 

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    DWORD fileSize = GetFileSize(hFile, NULL);

    std::cout << "\n" << fileSize << "\nPress Key Enter to run\n";
    std::cin.get();

    //3)Read szQuery from the file (all the file is reading) 
    //
    DWORD bytesRead = 0;
    char szQuery[500] = {'\0', };
    bool bReadFile = ReadFile(hFile, szQuery, fileSize, &bytesRead, NULL);

    //Query executed by "Child.exe"
    // 
    //4) Get the command: op and argOp
    char op[40] = { '\0', };//name of function
    float argOp = 0;        //argument    
    sscanf_s(szQuery, "%s%f", &op, 40, &argOp );//White characters are no value

    printf("From Child:Query:\n%s\n", szQuery);
    printf("op:<%s> argOp:<%7.4f>\n", op, argOp );

    //5) Check what is a name of function 
    if (0 == strcmp(op, "square"))
    {
        float fAnswer = pow(argOp, 2); //Execute the command
        //
        char szfAnswer[21] = { '\0', };
        size_t sizeAnswer = sizeof("fAnswer:") + 7 + 1; //Including '\0' at the end of "szfAnswer"
        sprintf_s(szfAnswer, sizeAnswer,"fAnswer:%7.4f\n", fAnswer); //Get the answer to szQuery

        SetFilePointer(hFile, 0, NULL, FILE_END);//Set Point of the file at the file end
        DWORD dwBytesWritten = 0;

        printf("fAnswer::%7.4f\n", fAnswer);
        std::cin.get();

        //Append the answer 
        bool bErrorFlag = WriteFile(
            hFile,           // open file handle
            szfAnswer,      // start of data to write
            sizeAnswer-1,  // number of bytes to write ,//Excluding '\0' at the end of "szfAnswer"
            &dwBytesWritten, // number of bytes that were written
            NULL); // no overlapped structure
        
        fileSize = GetFileSize(hFile, NULL);

    }
    

    std::cout <<"\n"<< fileSize << "\nPress Key Enter to quit\n";
    std::cin.get();

    CloseHandle(hFile);//Decrease the user count by 1! Before it was 2!
    return 0;
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
