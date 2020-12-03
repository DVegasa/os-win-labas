#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#include "windows.h"

using namespace std;

int main(void) {
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Создаём процессор
    if (0 == CreateProcess(
                 NULL,
                 "notepad.exe",
                 NULL,
                 NULL,
                 FALSE,
                 CREATE_SUSPENDED,  // Начинает поток спящим
                 0,
                 NULL,
                 &si,
                 &pi)) {
        cout << "#Error: CreateProccess: " << GetLastError();
        exit(0);
    }

    cout << "Successfully created: dwProcessId=" << pi.dwProcessId << endl;
    cout << "Successfully created: dwThreadId=" << pi.dwThreadId << endl;

    // Спим 10 секунд
    cout << "Before sleep" << endl;
    Sleep(10 * 1000);
    cout << "After sleep" << endl;

    // Продолжаем поток
    if (ResumeThread(pi.hThread) == (DWORD)-1) {
        cout << "#Error: ResumeThread: " << GetLastError();
    } else {
        cout << "Resume OK" << endl;
    }   

    // Спим, чтобы увидеть работающий процесс и не убивать его сразу
    Sleep(5 * 1000);

    // Получаем его приоритет
    cout << "GetThreadPriority = ";
    switch (GetThreadPriority(pi.hThread)) {
        case THREAD_PRIORITY_ABOVE_NORMAL:
            cout << "Priority 1 point above the priority class." << endl;
            break;
        case THREAD_PRIORITY_BELOW_NORMAL:
            cout << "Priority 1 point below the priority class." << endl;
            break;
        case THREAD_PRIORITY_HIGHEST:
            cout << "Priority 2 points above the priority class." << endl;
            break;
        case THREAD_PRIORITY_IDLE:
            cout << "Base priority of 1 for IDLE_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes, and a base priority of 16 for REALTIME_PRIORITY_CLASS processes." << endl;
            break;
        case THREAD_PRIORITY_LOWEST:
            cout << "Priority 2 points below the priority class." << endl;
            break;
        case THREAD_PRIORITY_NORMAL:
            cout << "Normal priority for the priority class." << endl;
            break;
        case THREAD_PRIORITY_TIME_CRITICAL:
            cout << "Base-priority level of 15 for IDLE_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes, and a base-priority level of 31 for REALTIME_PRIORITY_CLASS processes.." << endl;
            break;
    }

    // Завершаем процесс
    if (0 == TerminateProcess(pi.hProcess, NO_ERROR)) {
        cout << "#Error: TerminateProcess: " << GetLastError();
    } 

    // Закрываем хендлы
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    cout << "Terminated" << endl;
}