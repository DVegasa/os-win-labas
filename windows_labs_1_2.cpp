#include <exception>
#include <iostream>
#include <string>

#include "utils.cpp"
#undef UNICODE
#define UNICODE
#include "windows.h"

using namespace std;

HANDLE openFile(char path[255], bool accessWrite = false) {
    LPCTSTR lpFileName;                                 // file name
    DWORD dwDesiredAccess = GENERIC_READ;               // access mode
    DWORD dwShareMode = FILE_SHARE_READ;                // share mode
    LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;  // SD
    DWORD dwCreationDisposition = OPEN_EXISTING;        // how to create
    DWORD dwFlagsAndAttributes;                         // file attributes
    HANDLE hTemplateFile;                               // handle to template file

    if (accessWrite) {
        dwDesiredAccess |= GENERIC_WRITE;  // Разрешаем запись
        dwShareMode = 0;                   // Включаем монопольный доступ
    }

    HANDLE handle = CreateFile(
        LPCSTR(path),
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        NULL);

    if (handle == INVALID_HANDLE_VALUE) {
        stringstream ss;
        ss << "Не удалось открыть файл. Код ошибки: " << GetLastError();
        throw ss.str();
    }

    return handle;
}

HANDLE createFile(char path[255]) {
    HANDLE handle = CreateFile(
        LPCSTR(path),
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        NULL,
        NULL);

    if (handle == INVALID_HANDLE_VALUE) {
        stringstream ss;
        ss << "Не удалось создать файл. Код ошибки: " << GetLastError();
        throw ss.str();
    }
    return handle;
}

int main(void) {
    char pathFrom[255], pathTo[255];  // 255 -- максимальный путь в NTFS
    int n;
    bool toDelete;

    cout << "Введите адрес файла, откуда копировать:\n";
    cin >> pathFrom;
    cout << "Введите адрес файла, куда вставить:\n";
    cin >> pathTo;

    HANDLE hFrom;
    HANDLE hTo;
    try {
        hFrom = openFile(pathFrom, false);
        hTo = createFile(pathTo);
    } catch (string error) {
        cout << "Ошибка: " << error << endl;
        return -1;
    }

    cout << "С какого символа начать копирование?:\n";
    cin >> n;
    cout << "Удалить после копирования?: \n";
    cin >> toDelete;

    LARGE_INTEGER size;  // Размер файла в байтах (количество символов)
    GetFileSizeEx(hFrom, &size);
    cout << "Исходный файл содержит символов: " << size.QuadPart << endl;

    if (n > size.QuadPart) {
        cout << "Ошибка: в исходном файле всего " << size.QuadPart << " символов";
        CloseHandle(hFrom);
        CloseHandle(hTo);
        return -1;
    }

    int BUFFER_SIZE = size.QuadPart + 1;  // Максимальное кол-во символов в буфере
    char ReadBuffer[BUFFER_SIZE] = {0};   // Тут будут символы из файла
    OVERLAPPED ol = {0};                  // Магическая штука, без которой не работает

    ReadFileEx(hFrom, ReadBuffer, BUFFER_SIZE - 1, &ol, NULL);
    WriteFileEx(hTo, &ReadBuffer[n], BUFFER_SIZE - 1 - n, &ol, NULL);

    cout << "Успешно скопировано символов: " << BUFFER_SIZE - 1 - n << endl;
    // Закрываем хендлы
    CloseHandle(hFrom);
    CloseHandle(hTo);

    if (toDelete) {  // Если надо, то удаляем исходный файл
        DeleteFile((LPCSTR)pathFrom);
    }
    
    return 0;
}