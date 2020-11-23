#include <iostream>
#include <string>

#include "utils.cpp"
#include "windows.h"

using namespace std;

string ERR_ALLOCATION = string("Ошибка при аллокации памяти");

DWORD getPageSize() {
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    return siSysInfo.dwPageSize;
}

LPVOID doAlloc(int n) {
    LPVOID lpAddress;
    if (VirtualAlloc(lpAddress, getPageSize() * n, MEM_RESERVE, PAGE_READWRITE) == NULL) {
        throw ERR_ALLOCATION;
    }
    return lpAddress;
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
    cout << "Начальный адрес: " << lpBase << endl;
    // 2
    return 0;
}