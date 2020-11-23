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

void commit(LPVOID& lpAddress, int n, int shift, int flag = PAGE_READWRITE) {
    lpAddress = lpAddress + getPageSize() * shift;

    if (VirtualAlloc(lpAddress, getPageSize() * n, MEM_COMMIT, flag) == NULL) {
        throw ERR_COMMIT;
    }
}

string stateToString(int s) {
    switch (s) {
        case MEM_COMMIT:
            return "MEM_COMMIT";
        case MEM_FREE:
            return "MEM_FREE";
        case MEM_RESERVE:
            return "MEM_RESERVE";
        default:
            return "???";
    }
}

string protectToString(int p) {
    switch (p) {
        case PAGE_EXECUTE:
            return "PAGE_EXECUTE";
        case PAGE_EXECUTE_READ:
            return "PAGE_EXECUTE_READ";
        case PAGE_EXECUTE_READWRITE:
            return "PAGE_EXECUTE_READWRITE";
        case PAGE_EXECUTE_WRITECOPY:
            return "PAGE_EXECUTE_WRITECOPY";
        case PAGE_NOACCESS:
            return "PAGE_NOACCESS";
        case PAGE_READONLY:
            return "PAGE_READONLY";
        case PAGE_READWRITE:
            return "PAGE_READWRITE";
        case PAGE_WRITECOPY:
            return "PAGE_WRITECOPY";
        case 0:
            return "NO ACCESS";
        default:
            return "???";
    }
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

    cout << "С какой страницы начать коммитить (индексация с нуля)?" << endl;
    int from;
    cin >> from;
    if (from + nCommit > n) {
        cout << "Ты выйдешь за границу. Программа завершается" << endl;
        return 0;
    }

    LPVOID lpComitted = lpBase;
    commit(lpComitted, nCommit, from);
    cout << "Коммиты начинаются с: " << lpComitted << endl;

    // 3
    const int SIZE = 3;
    DWORD a[SIZE] = {42, 100, 24};
    LPDWORD pointer = (LPDWORD)lpComitted;
    errno_t err = memcpy_s(pointer, sizeof(pointer) * SIZE, a, sizeof(DWORD) * SIZE);
    if (err == EINVAL) {
        cout << "Ошибка при memcpy_s EINVAL: " << err << endl;
    }
    if (err == ERANGE) {
        cout << "Ошибка при memcpy_s ERANGE: " << err << endl;
    }

    cout << "Содержимое pointer: " << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << pointer[i] << endl;
    }
    cout << "-----------" << endl;

    // 4
    cout << "Инфа про коммитный участок: " << endl;
    cout << "Адрес: " << lpComitted << endl;
    MEMORY_BASIC_INFORMATION lpBuffer;
    if (VirtualQuery(lpComitted, &lpBuffer, sizeof(lpBuffer)) == 0) {
        cout << "Ошибка VirtualQuery 1: " << GetLastError() << endl;
    }
    cout << "State: " << stateToString(lpBuffer.State) << endl;
    cout << "Protect: " << protectToString(lpBuffer.Protect) << endl
         << endl;

    if (from + nCommit >= n) {
        cout << "Следующая страница находится вне зарезерированной памяти" << endl;
    }
    cout << "Инфа про страницу за коммитным участком: " << endl;
    MEMORY_BASIC_INFORMATION lpBuffer2;
    LPVOID p = lpComitted + getPageSize() * (nCommit);
    cout << "Адрес: " << p << endl;
    if (VirtualQuery(p, &lpBuffer2, sizeof(lpBuffer2)) == 0) {
        cout << "Ошибка VirtualQuery 2: " << GetLastError() << endl;
    }
    cout << "State: " << stateToString(lpBuffer2.State) << endl;
    cout << "Protect: " << protectToString(lpBuffer2.Protect) << endl
         << endl;

    // 5
    if (from + nCommit < n) {
        MEMORY_BASIC_INFORMATION lpBuffer3;
        LPVOID p2 = lpComitted + getPageSize() * (nCommit);
        commit(p2, 1, 0, PAGE_READONLY);
        cout << "Закоммиченно ещё" << endl;

        cout << "Адрес: " << p2 << endl;
        if (VirtualQuery(p2, &lpBuffer3, sizeof(lpBuffer3)) == 0) {
            cout << "Ошибка VirtualQuery 3: " << GetLastError() << endl;
        }
        cout << "State: " << stateToString(lpBuffer3.State) << endl;
        cout << "Protect: " << protectToString(lpBuffer3.Protect) << endl
             << endl;
    } else {
        cout << "Следующая страница не зарезервирована. Пропуск 5 шага" << endl;
    }
    // 6
    cout << "Сколько страниц освободить?" << endl;
    int nToDecommit;
    cin >> nToDecommit;
    cout << "С какой страницы (индексация с нуля)?" << endl;
    int toDecommitFrom;
    cin >> toDecommitFrom;

    if (nToDecommit < 0 || toDecommitFrom < 0) {
        cout << "Ошибка: отрицательные числа недопустимы" << endl;
        return 0;
    }

    if (toDecommitFrom >= n) {
        cout << "Ошибка: стартовое значение больше выделенных страниц" << endl;
        return 0;
    }

    // Если для освобождения хотят больше страниц чем выделенно, число округлится
    // до максимально возможного с данной страницы toDecommitFrom
    int maxToFree = n - toDecommitFrom;
    if (toDecommitFrom > maxToFree) toDecommitFrom = maxToFree;
    LPVOID pDecommit = lpBase + (getPageSize() * toDecommitFrom);
    if (VirtualFree(pDecommit, getPageSize() * toDecommitFrom, MEM_DECOMMIT) == 0) {
        cout << "Ошибка VirtualFree: " << GetLastError() << endl;
        return 0;
    }
    cout << "Декоммит с адреса: " << pDecommit << endl;

    MEMORY_BASIC_INFORMATION lpBuffer4;
    if (VirtualQuery(pDecommit, &lpBuffer4, sizeof(lpBuffer4)) == 0) {
        cout << "Ошибка VirtualQuery 4: " << GetLastError() << endl;
    }
    cout << "State: " << stateToString(lpBuffer4.State) << endl;
    cout << "Protect: " << protectToString(lpBuffer4.Protect) << endl
         << endl;

    // 7
    cout << "Релиз памяти с адреса: " << lpBase << endl;
    if (VirtualFree(lpBase, 0, MEM_RELEASE) == 0) {
        cout << "Ошибка VirtualFree: " << GetLastError() << endl;
        return 0;
    }
    return 0;
}