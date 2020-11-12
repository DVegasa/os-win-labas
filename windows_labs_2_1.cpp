#include <iostream>
#include <string>
#include <thread>

#include "utils.cpp"
#include "windows.h" 

void printRamInfo() {
    while (true) {
        MEMORYSTATUS statex;
        statex.dwLength = sizeof (statex);
        GlobalMemoryStatus(&statex);

        cout << "Загрузка: " << statex.dwMemoryLoad << "%" << endl;
        cout << "Доступно: "
             << stringedSize(statex.dwAvailPhys)
             << " (" << statex.dwAvailPhys << " B)"
             << " из "
             << stringedSize(statex.dwTotalPhys)
             << " (" << statex.dwTotalPhys << " B)"
             << endl << endl;

        Sleep(1000);
    }
}

int main(void) {
    thread thread(printRamInfo);
    thread.detach();

    while (true) {
        char key;
        cin >> key;
        
        if (key == 'q') {
            return 0;
        }
    }
}