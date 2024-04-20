// Main.cpp
#include <windows.h>
#include <iostream>

// Определения функций из оригинального проекта
BOOL GetFileEncripted(LPOPENFILENAME lpofn);
BOOL EncriptFileToFile(LPSTR lpstrSourceFile, LPSTR lpstrDestinationFile);
bool Simplest(unsigned uLengthKey, LPSTR szKey, HANDLE hDestFile);
bool CopyFileToFile(HANDLE hSourceFile, HANDLE hDestFile);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <source_file> <destination_file>" << std::endl;
        return 1;
    }

    // Устанавливаем текущую директорию в директорию проекта FileToFile
    SetCurrentDirectory("путь_к_папке_проекта_FileToFile");

    // Аргументы командной строки
    LPSTR lpstrSourceFile = argv[1];
    LPSTR lpstrDestinationFile = argv[2];

    // Вызываем GetFileEncripted, используя адаптированную версию
    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(OPENFILENAME));

    // Передаем имя исходного файла
    strcpy(ofn.lpstrFile, lpstrSourceFile);
    ofn.nMaxFile = strlen(lpstrSourceFile) + 1;

    // Вызываем функцию EncriptFileToFile
    if (EncriptFileToFile(lpstrSourceFile, lpstrDestinationFile)) {
        std::cout << "Encryption successful." << std::endl;
    }
    else {
        std::cerr << "Encryption failed." << std::endl;
    }

    return 0;
}
