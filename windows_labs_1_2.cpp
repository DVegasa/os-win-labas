#include <exception>
#include <iostream>
#include <string>

#include "utils.cpp"
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
        ss << "OpenFileError: " << GetLastError();
        throw ss.str();
    }

    return handle;
}

string _readFile(HANDLE handle, int symbFrom) {
    return NULL;
}

void doCopy(char pathFrom[255], char pathTo[255], int symbFrom) {
    HANDLE handleFrom;
    HANDLE handleTo;

    try {
        handleFrom = openFile(pathFrom, false);
        handleTo = openFile(pathTo, true);
    } catch (string error) {
        cout << error;
    }

    // string buffer = _readFile(handleFrom, symbFrom);
}

void deleteFile(char path[255]) {
}

int main(void) {
    char pathFrom[255], pathTo[255];
    int symbFrom;
    bool toDelete;


    cout << "Введите адрес файла, откуда копировать:\n";
    cin >> pathFrom;

    try {
        HANDLE test = openFile(pathFrom, false);
    } catch (string error) {
        cout << error << endl;
    }
    cout << "OK" << endl;
    return 0;



    cout << "Введите адрес файла, куда вставить:\n";
    cin >> pathTo;
    cout << "С какого символа начать копирование?:\n";
    cin >> symbFrom;
    cout << "Удалить после копирования?: \n";
    cin >> toDelete;

    doCopy(pathFrom, pathTo, symbFrom);

    if (toDelete) {
        deleteFile(pathFrom);
    }
    return 0;
}