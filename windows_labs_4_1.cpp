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
                 NULL,           // имя исполняемого модуля
                 "notepad.exe",  // Командная строка
                 NULL,           // Указатель на структуру SECURITY_ATTRIBUTES
                 NULL,           // Указатель на структуру SECURITY_ATTRIBUTES
                 FALSE,          // Флаг наследования текущего процесса
                 0,              // Флаги способов создания процесса
                 0,              // Указатель на блок среды
                 NULL,           // Текущий диск или каталог
                 &si,            // Указатель нас структуру STARTUPINFO
                 &pi             // Указатель нас структуру PROCESS_INFORMATION
                 )) {
        cout << "#Error: CreateProccess: " << GetLastError();
        exit(0);
    }

    cout << "Successfully created: dwProcessId=" << pi.dwProcessId << endl;
    cout << "Successfully created: dwThreadId=" << pi.dwThreadId << endl;

    bool isSuspended = false;
    bool isClosed = false;
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
                break;
            } else {
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                isClosed = true;
                cout << "Terminated" << endl;
                break;
            }

        } else if (c == 's') {
            if (SuspendThread(pi.hThread) == (DWORD)-1) {
                cout << "#Error: SuspendThread: " << GetLastError();
                break;
            } else {
                cout << "Suspended OK" << endl;
            }

        } else if (c == 'r') {
            if (ResumeThread(pi.hThread) == (DWORD)-1) {
                cout << "#Error: ResumeThread: " << GetLastError();
                break;
            } else {
                cout << "Resume OK" << endl;
            }
        }
    }

    if (isClosed == false) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        isClosed = true;
    }
    cout << "Global exit" << endl;
}