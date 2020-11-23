#include <iostream>
#include <string>

#include "utils.cpp"
#include "windows.h"

using namespace std;

string ERR_ALLOCATION = string("Ошибка при аллокации памяти");
string ERR_COMMIT = string("Ошибка при коммите памяти");

DWORD getPageSize() {
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    return siSysInfo.dwPageSize;
}

LPVOID doAlloc(int n) {
    LPVOID lpAddress = VirtualAlloc(NULL, getPageSize() * n, MEM_RESERVE, PAGE_READWRITE);
    if (lpAddress == NULL) {
        throw ERR_ALLOCATION;
    }
    return lpAddress;
}

void commit(LPVOID& lpAddress, int n, int shift) {
    // cout << "#commit before: " << lpAddress << endl;
    lpAddress = lpAddress + getPageSize() * shift;

    if (VirtualAlloc(lpAddress, getPageSize() * n, MEM_COMMIT, PAGE_READWRITE) == NULL) {
        throw ERR_COMMIT;
    }
    // cout << "#commit after: " << lpAddress << endl;
}

int main(void) {
    // 1
    cout << "Размер страницы в данной системе: " << stringedSize(getPageSize()) << endl;
    cout << "Введите кол-во страниц для резервации: " << endl;
    int n;
    cin >> n;
    LPVOID lpBase;
    try {
        lpBase = doAlloc(n);
    } catch (string s) {
        cout << s << endl;
        return -1;
    }
    cout << "Начальный адрес резервов: " << lpBase << endl;

    // 2
    cout << "Введите кол-во страниц для коммита: " << endl;
    int nCommit;
    cin >> nCommit;

    if (nCommit > n) {
        cout << "Зарезервировано только " << n << " страница. Программа завершается" << endl;
        return 0;
    }

    cout << "С какой страницы начать коммитить?" << endl;
    int from;
    cin >> from;
    if (from + nCommit > n) {
        cout << "Ты выйдешь за границу. Программа завершается" << endl;
        return 0;
    }

    LPVOID lpComitted = lpBase;
    commit(lpComitted, nCommit, from);
    cout << "Коммиты начинаются с: " << lpComitted << endl;

    return 0;
    // 3
    const int SIZE = 3;
    int a[SIZE] = {42, -100, 24}; // 32 bytes
    LPDWORD pointer = (LPDWORD) lpComitted;
    errno_t err = memcpy_s(pointer, sizeof(pointer), a, sizeof(int) * SIZE);
    if (err) {
        cout << "Ошибка при memcpy_s" << endl;
        return 0;
    }
    return 0;
}