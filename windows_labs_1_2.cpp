#include <exception>
#include <iostream>
#include <string>

#include "utils.cpp"
#include "windows.h"

using namespace std;

class ReadError : exception {};

void askForData(string &pathFrom, string &pathTo, int &symbFrom) {
    cout << "Введите адрес файла, откуда копировать:\n";
    cin >> pathFrom;
    cout << "Введите адрес файла, куда вставить:\n";
    cin >> pathTo;
    cout << "С какого символа начать копирование?:\n";
    cin >> symbFrom;
}

string _readFile(string pathFrom, int symbFrom) {
    DWORD numberOfBytesRead;

    LPVOID lpBuffer;
    DWORD nNumberOfBytesToRead;
    LPDWORD lpNumberOfBytesRead = &numberOfBytesRead;

    // Тут нужно открыть файл через CreateFile и получить HANDLE файла

    HANDLE readResult = ReadFile(

        GENERIC_READ,          // дескриптор файла
        lpBuffer,              // буфер данных
        nNumberOfBytesToRead,  // количество байт для прочтения
        lpNumberOfBytesRead,   // количество фактически прочитанных байт
        NULL);                 // указатель на структуру, используемую при асинхронном чтении

    if (readResult == INVALID_HANDLE_VALUE) {
        CloseHandle(readResult);
        throw ReadError();
    }
}

void startCopying(string pathFrom, string pathTo, int symbFrom) {
    try {
        string buffer = _readFile(pathFrom, symbFrom);
    } catch (ReadError readError) {
        cout << "Ошибка чтения файла: " << GetLastError();
        cout << endl;
    }
}

int main(void) {
    string pathFrom;
    string pathTo;
    int symbFrom;
    askForData(pathFrom, pathTo, symbFrom);
    startCopying(pathFrom, pathTo, symbFrom);
    return 0;
}