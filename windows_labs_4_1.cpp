#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#include "utils.cpp"
#include "windows.h"

int main(void) {
    cout << "--- init ---" << endl;

    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (0 == CreateProcess(
                 NULL,        // имя исполняемого модуля
                 "notepad.exe",  // Командная строка
                 NULL,        // Указатель на структуру SECURITY_ATTRIBUTES
                 NULL,        // Указатель на структуру SECURITY_ATTRIBUTES
                 FALSE,       // Флаг наследования текущего процесса
                 0,           // Флаги способов создания процесса
                 0,           // Указатель на блок среды
                 NULL,        // Текущий диск или каталог
                 &si,         // Указатель нас структуру STARTUPINFO
                 &pi          // Указатель нас структуру PROCESS_INFORMATION
                 )) {
        cout << "#Error: CreateProccess: " << GetLastError();
        exit(0);
    }

    cout << "Successfully created: dwProcessId=" << pi.dwProcessId << endl;
    cout << "Successfully created: dwThreadId=" << pi.dwThreadId << endl;

    while (true) {
        cout << endl;
        cout << "s: SuspendThread()" << endl;
        cout << "r: ResumeThread()" << endl;
        cout << "t: TerminateProcess" << endl;
        cout << "Input command: " << endl;
        char c;
        cin >> c;

        if (c == 't') {
            if (0 == TerminateProcess(pi.hProcess, NO_ERROR)) {
                cout << "#Error: TerminateProcess: " << GetLastError();
                exit(0);
            } else {
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                cout << "Terminated" << endl;
                break;
            }
        }
    }

    cout << "Global exit" << endl;
}