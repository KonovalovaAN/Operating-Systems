#include <windows.h>
#include <iostream>

bool CopyFileToFile(const std::wstring& sourceFile, const std::wstring& destFile);

int main(int argc, wchar_t* argv[]) {
    if (argc < 3) {
        std::wcerr << L"Usage: " << argv[0] << L" <source_file> <destination_file>" << std::endl;
        std::wcerr << L"Please provide the required command-line arguments." << std::endl;
        return 1;
    }

    std::wstring sourceFile = argv[1];
    std::wstring destFile = argv[2];

    if (CopyFileToFile(sourceFile, destFile)) {
        std::wcout << L"File copy successful." << std::endl;
    }
    else {
        std::wcerr << L"File copy failed." << std::endl;
    }

    return 0;
}

bool CopyFileToFile(const std::wstring& sourceFile, const std::wstring& destFile) {
    HANDLE hSourceFile = CreateFile(sourceFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSourceFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Error opening source file." << std::endl;
        return false;
    }

    HANDLE hDestFile = CreateFile(destFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hDestFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Error creating destination file." << std::endl;
        CloseHandle(hSourceFile);
        return false;
    }

    const int bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead, bytesWritten;

    while (ReadFile(hSourceFile, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
        WriteFile(hDestFile, buffer, bytesRead, &bytesWritten, NULL);

        if (bytesRead != bytesWritten) {
            std::wcerr << L"Error writing to destination file." << std::endl;
            CloseHandle(hSourceFile);
            CloseHandle(hDestFile);
            return false;
        }
    }

    CloseHandle(hSourceFile);
    CloseHandle(hDestFile);

    return true;
}
