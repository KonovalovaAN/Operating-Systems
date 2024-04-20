#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"
#include <string>

char szPagingFileShareName[] = "{11FB95B0-4300-49fb-BE12-B086FD00D7B8}";
char szEventCharName[] = "{D244D5E4-4640-4186-BCC2-701BDE8E26DC}";
char szEventTerminationName[] = "{F3358C89-E4AD-43f4-8D20-38A038F47459}";

int main(int argc, char* argv[])
{
    HANDLE  hEventChar,
        hEventTermination,
        hPagingFileMapping;

    hEventTermination = CreateEvent(NULL, TRUE, FALSE, szEventTerminationName);
    if (!hEventTermination) {
        printf("Create Event <%s>: Error %ld\n", szEventTerminationName, GetLastError());
        printf("Press any key to quit...\n");
        getch();
        return 0;
    }

    hEventChar = CreateEvent(NULL, FALSE, FALSE, szEventCharName);
    if (!hEventChar) {
        printf("Create Event <%s>: Error %ld\n", szEventCharName, GetLastError());
        printf("Press any key to quit...\n");
        getch();
        return 0;
    }

    hPagingFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096, szPagingFileShareName);
    if (!hPagingFileMapping) {
        printf("Create File Mapping <%s>: Error %ld\n", szPagingFileShareName, GetLastError());
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

    printf("\nPgFlServer is ready to receive input from PgFlClient...\n");
    printf("Press Esc to quit...\n");

    bool bTerminate = false;
    std::string receivedString;

    while (!bTerminate)
    {
        DWORD dwWaitResult = WaitForSingleObject(hEventChar, INFINITE);

        if (dwWaitResult == WAIT_OBJECT_0) {
            int length = *((int*)lpFileMap + 1020);
            receivedString.clear();

            // Получить строку
            for (int i = 0; i < length; i++) {
                receivedString += *((LPSTR)lpFileMap + i);
            }

            if (receivedString == "server shoot down") {
                bTerminate = true;
            }
            else {
                // Инвертировать строку
                std::reverse(receivedString.begin(), receivedString.end());

                // Вывести инвертированную строку
                printf("Received string: %s\n", receivedString.c_str());
            }

            ResetEvent(hEventChar);
        }

        if (_kbhit() && _getch() == 27) { // Esc key
            bTerminate = true;
        }
    }

    CloseHandle(hEventTermination);
    CloseHandle(hEventChar);
    UnmapViewOfFile(lpFileMap);
    CloseHandle(hPagingFileMapping);

    printf("PgFlServer closed...\n");
    printf("Press any key to quit...\n");
    getch();
    return 0;
}