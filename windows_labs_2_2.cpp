#include <iostream>
#include <string>

#include "utils.cpp"
#include "windows.h" 

using namespace std;

DWORD getPageSize() {
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    return siSysInfo.dwPageSize;
}

int main(void) {
    cout << "Размер страницы в данной системе: ";
    cout << stringedSize(getPageSize()) << endl;
    cout << "Кол-во страниц для резервации: " << endl;
    int n;
    cin >> n;
    return 0;
}