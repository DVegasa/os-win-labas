#include <exception>
#include <iostream>
#include <string>

#include "utils.cpp"
#include "windows.h"

using namespace std;

string getLastErrorStr() {
    //Get the error message, if any.
    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0)
        return std::string();  //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

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
    char pathFrom[255], pathTo[255];
    int symbFrom;
    bool toDelete;

    cout << "Введите адрес файла, откуда копировать:\n";
    cin >> pathFrom;
    cout << "Введите адрес файла, куда вставить:\n";
    cin >> pathTo;
    // cout << "С какого символа начать копирование?:\n";
    // cin >> symbFrom;
    // cout << "Удалить после копирования?: \n";
    // cin >> toDelete;

    HANDLE hFrom;
    HANDLE hTo;
    try {
        hFrom = openFile(pathFrom, false);
        hTo = createFile(pathTo);
    } catch (string error) {
        cout << "Ошибка: " << error << endl;
        return -1;
    }

    

    CloseHandle(hFrom);
    CloseHandle(hTo);

    cout << "OK" << endl;

    return 0;
}