#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <thread>

#include "windows.h"

using namespace std;

DWORD a(LPVOID _) {
    for (int i = 0; i < 10; i++) {
        cout << "a" + to_string(i) + ":";
        for (char c = 'a'; c <= 'z'; c++) {
            cout << c;
            Sleep(10);
        }
        cout << endl;
    }
    return 0;
}

DWORD b(LPVOID _) {
    for (int i = 0; i < 10; i++) {
        cout << "b" + to_string(i) + ":";
        for (char c = '0'; c <= '9'; c++) {
            cout << c;
            Sleep(10);
        }
        cout << endl;
    }
    return 0;
}

int main(void) {
    // Проверяем каждую функцию при синхронном запуске
    cout << "Проверяем каждую функцию при синхронном запуске" << endl;
    a(LPVOID());
    b(LPVOID());
    cout << "--------" << endl;

    // Создаём первый поток
    DWORD aInfo;
    HANDLE aThread = CreateThread(
        NULL,
        0,
        a,
        NULL,
        0,
        &aInfo);
    if (aThread == NULL) {
        cout << "#error: CreateThread@a: " << GetLastError();
        ExitProcess(0);  // This will automatically clean up threads and memory.
    }

    // Создаём второй поток
    DWORD bInfo;
    HANDLE bThread = CreateThread(
        NULL,
        0,
        b,
        NULL,
        0,
        &bInfo);
    if (bThread == NULL) {
        cout << "#error: CreateThread@b: " << GetLastError();
        ExitProcess(0);  // This will automatically clean up threads and memory.
    }

    // Ждём их
    HANDLE handles[2];
    handles[0] = aThread;
    handles[1] = bThread;
    cout << "\nBefore WaitForMultipleObjects\n";
    WaitForMultipleObjects(2, handles, TRUE, INFINITE);
    cout << "\nAfter WaitForMultipleObjects\n";
}