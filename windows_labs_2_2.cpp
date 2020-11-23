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
    LPVOID lpAddress;
    if (VirtualAlloc(lpAddress, getPageSize() * n, MEM_RESERVE, PAGE_READWRITE) == NULL) {
        throw ERR_ALLOCATION;
    }
    return lpAddress;
}

void commit(LPVOID& lpAddress, int n, int shift) {
    cout << "#commit before: " << lpAddress << endl;
    lpAddress = lpAddress + getPageSize() * shift;
    
    if (VirtualAlloc(lpAddress, getPageSize() * n, MEM_COMMIT, PAGE_READWRITE) == NULL) {
        throw ERR_COMMIT;
    }
    cout << "#commit after: " << lpAddress << endl;
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
    cout << "Введите кол-во страниц для коммита: " << endl;
    int nCommit;
    cin >> nCommit;
    LPVOID lp2 = lpBase;
    commit(lp2, nCommit, 1);
    return 0;
}