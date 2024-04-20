// Main.cpp
#include <windows.h>
#include <iostream>

// ����������� ������� �� ������������� �������
BOOL GetFileEncripted(LPOPENFILENAME lpofn);
BOOL EncriptFileToFile(LPSTR lpstrSourceFile, LPSTR lpstrDestinationFile);
bool Simplest(unsigned uLengthKey, LPSTR szKey, HANDLE hDestFile);
bool CopyFileToFile(HANDLE hSourceFile, HANDLE hDestFile);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <source_file> <destination_file>" << std::endl;
        return 1;
    }

    // ������������� ������� ���������� � ���������� ������� FileToFile
    SetCurrentDirectory("����_�_�����_�������_FileToFile");

    // ��������� ��������� ������
    LPSTR lpstrSourceFile = argv[1];
    LPSTR lpstrDestinationFile = argv[2];

    // �������� GetFileEncripted, ��������� �������������� ������
    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(OPENFILENAME));

    // �������� ��� ��������� �����
    strcpy(ofn.lpstrFile, lpstrSourceFile);
    ofn.nMaxFile = strlen(lpstrSourceFile) + 1;

    // �������� ������� EncriptFileToFile
    if (EncriptFileToFile(lpstrSourceFile, lpstrDestinationFile)) {
        std::cout << "Encryption successful." << std::endl;
    }
    else {
        std::cerr << "Encryption failed." << std::endl;
    }

    return 0;
}
