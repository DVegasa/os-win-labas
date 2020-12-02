#include <iostream>
#include <string>

#include "utils.cpp"
#include "windows.h"

int main(void) {
    LPCSTR lpApplicationName = NULL;
    LPSTR lpCommandLine = "calc.exe"; 
    LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
    LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
    BOOL bInheritHandles = FALSE;  // возможно нужно будет менять
    DWORD dwCreationFlags = 0;
    LPVOID lpEnvironment = NULL;
    LPCSTR lpCurrentDirectory = NULL;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (0 == CreateProcessA(
                 lpApplicationName,
                 lpCommandLine,
                 lpProcessAttributes,
                 lpThreadAttributes,
                 bInheritHandles,
                 dwCreationFlags,
                 lpEnvironment,
                 lpCurrentDirectory,
                 &si,
                 &pi)) {
        error("CreateProcessA: " + GetLastError());
    }

    print("Successfully created:\ndwProcessId=" + pi.dwProcessId);

    while (true) {
        print();
        print("s: SuspendThread()");
        print("r: ResumeThread()");
        print("t: TerminateProcess");
        print("Input command: ");
        char c;
        cin >> c;

        if (c == 't') {
            TerminateProcess(pi.hProcess, 0);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            print("Terminated");
            break;
        }
    }

    print("Global exit");
}