#include <iostream>
#include <string>

#include "utils.cpp"
#include "windows.h"

int main(void) {
    // #region a
    // LPCSTR lpApplicationName = NULL;
    // LPSTR lpCommandLine = "calc.exe";
    // LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
    // LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
    // BOOL bInheritHandles = FALSE;  // возможно нужно будет менять
    // DWORD dwCreationFlags = 0;
    // LPVOID lpEnvironment = NULL;
    // LPCSTR lpCurrentDirectory = NULL;
    // STARTUPINFOA si;
    // PROCESS_INFORMATION pi;
//
    // ZeroMemory(&si, sizeof(si));
    // si.cb = sizeof(si);
    // ZeroMemory(&pi, sizeof(pi));
//
    // if (0 == CreateProcessA(
    //              lpApplicationName,
    //              lpCommandLine,
    //              lpProcessAttributes,
    //              lpThreadAttributes,
    //              bInheritHandles,
    //              dwCreationFlags,
    //              lpEnvironment,
    //              lpCurrentDirectory,
    //              &si,
    //              &pi)) {
    //     error("CreateProcessA: " + GetLastError());
    // }
    // #endregion a

    // #region normal
    // --------------------------
    LPCTSTR lpApplicationName = NULL;
    LPTSTR lpCommandLine = "calc.exe";
    LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
    LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
    BOOL bInheritHandles = FALSE;
    DWORD dwCreationFlags = 0;
    LPVOID lpEnvironment = NULL;
    LPCTSTR lpCurrentDirectory = NULL;
    STARTUPINFO lpStartupInfo;
    PROCESS_INFORMATION pi;
    ZeroMemory(&lpStartupInfo, sizeof(lpStartupInfo));
    lpStartupInfo.cb = sizeof(lpStartupInfo);
    ZeroMemory(&pi, sizeof(pi));
    if (0 == CreateProcess(
        lpApplicationName,    // имя исполняемого модуля
        lpCommandLine,        // Командная строка
        lpProcessAttributes,  // Указатель на структуру SECURITY_ATTRIBUTES
        lpThreadAttributes,   // Указатель на структуру SECURITY_ATTRIBUTES
        bInheritHandles,      // Флаг наследования текущего процесса
        dwCreationFlags,      // Флаги способов создания процесса
        lpEnvironment,        // Указатель на блок среды
        lpCurrentDirectory,   // Текущий диск или каталог
        &lpStartupInfo,       // Указатель нас структуру STARTUPINFO
        &pi                   // Указатель нас структуру PROCESS_INFORMATION
    )) {
         cout << "#Error: CreateProccess: " << GetLastError();
         exit(0);
    }
    // ------------------------
    // #endregion normal

    WaitForSingleObject(pi.hProcess, INFINITE);
    Sleep(100);
    cout << "Successfully created: dwProcessId=" << pi.dwProcessId << endl;
    cout << "Successfully created: dwThreadId=" << pi.dwThreadId << endl;

    while (true) {
        print();
        print("s: SuspendThread()");
        print("r: ResumeThread()");
        print("t: TerminateProcess");
        print("Input command: ");
        char c;
        cin >> c;

        if (c == 't') {
            if (0 == TerminateProcess(pi.hProcess, NO_ERROR)) {
                cout << "#Error: TerminateProcess: " << GetLastError();
                exit(0);
            }
            else {
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                print("Terminated");
                break;
            }
        }
    }

    print("Global exit");
}