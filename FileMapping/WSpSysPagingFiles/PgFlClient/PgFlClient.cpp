#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"
#include <string>
#include <iostream>

char szPagingFileShareName[] = "{11FB95B0-4300-49fb-BE12-B086FD00D7B8}";
char szEventCharName[] = "{D244D5E4-4640-4186-BCC2-701BDE8E26DC}";
char szEventTerminationName[] = "{F3358C89-E4AD-43f4-8D20-38A038F47459}";

int main(int argc, char* argv[])
{
    HANDLE  hEventChar,
        hEventTermination,
        hPagingFileMapping;

    hEventTermination = OpenEvent(EVENT_ALL_ACCESS, FALSE, szEventTerminationName);
    if (!hEventTermination) {
        printf("Open Event <%s>: Error %ld\n", szEventTerminationName, GetLastError());
        printf("Press any key to quit...\n");
        getch();
        return 0;
    }

    if (ERROR_ALREADY_EXISTS == GetLastError()) {
        printf("PgFlServer has already started\n");
        printf("Press any key to continue...\n");
        getch();
    }

    hEventChar = OpenEvent(EVENT_ALL_ACCESS, FALSE, szEventCharName);
    if (!hEventChar) {
        printf("Open Event <%s>: Error %ld\n", szEventCharName, GetLastError());
        printf("Press any key to quit...\n");
        getch();
        return 0;
    }

    hPagingFileMapping = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, szPagingFileShareName);
    if (!hPagingFileMapping) {
        printf("Open File Mapping <%s>: Error %ld\n", szPagingFileShareName, GetLastError());
        printf("Press any key to quit...\n");
        getch();
        return 0;
    }

    LPVOID lpFileMap = MapViewOfFile(hPagingFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if (!lpFileMap) {
        printf("Map View Of File <%s>: Error %ld\n", szPagingFileShareName, GetLastError());
        printf("Press any key to quit...\n");
        getch();
        return 0;
    }

    printf("\nPgFlClient is ready to send input to PgFlServer...\n");
    printf("Type a string or enter 'server shoot down' to terminate ...\n");

    bool bTerminate = false;
    std::string inputString;

    while (!bTerminate)
    {
        std::getline(std::cin, inputString);

        if (inputString == "server shoot down") {
            SetEvent(hEventTermination);
            bTerminate = true;
        }

        size_t length = inputString.length();
        if (length > 0) {
            // Отправить длину строки
            *((int*)lpFileMap + 1020) = static_cast<int>(length);

            // Отправить строку
            for (size_t i = 0; i < length; i++) {
                *((LPSTR)lpFileMap + i) = inputString[i];
            }

            SetEvent(hEventChar);
        }
    }

    CloseHandle(hEventTermination);
    CloseHandle(hEventChar);
    UnmapViewOfFile(lpFileMap);
    CloseHandle(hPagingFileMapping);

    printf("PgFlClient closed...\n");
    printf("Press any key to quit...\n");
    getch();
    return 0;
} 