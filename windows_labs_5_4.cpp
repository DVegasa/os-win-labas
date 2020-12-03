#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <thread>

#include "windows.h"

using namespace std;

HANDLE mutex;

DWORD a(LPVOID _) {
    for (int i = 0; i < 10; i++) {
        WaitForSingleObject(mutex, INFINITE);
        cout << "a" + to_string(i) + ":";
        for (char c = 'a'; c <= 'z'; c++) {
            cout << c;
            Sleep(10);
        }
        cout << endl;
        ReleaseMutex(mutex);
    }
    return 0;
}

DWORD b(LPVOID _) {
    for (int i = 0; i < 10; i++) {
        WaitForSingleObject(mutex, INFINITE);
        cout << "b" + to_string(i) + ":";
        for (char c = '0'; c <= '9'; c++) {
            cout << c;
            Sleep(10);
        }
        cout << endl;
        ReleaseMutex(mutex);
    }
    return 0;
}

int main(void) {
    mutex = CreateMutex(0, 0, 0);

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
    /*
     Иногда может быть такой вывод
        a0:a
        Before WaitForMultipleObjects
        bcdefghijklmnopqrstuvwxyz
     он связан с тем, что в объект cout передача происходит одновременно и из потока а,
     и из основного потока main. Чтобы это исправить сюда тоже можно сделать синхронизацию
     на мютексе, но я её не стал делать чтобы избежать усложнения кода нерелавнтными целями
    */
    WaitForMultipleObjects(2, handles, TRUE, INFINITE);
    cout << "\nAfter WaitForMultipleObjects\n";
    CloseHandle(mutex);  // Закрываем мютекс
}