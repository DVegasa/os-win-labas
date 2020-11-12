#include <iostream>
#include <string>
#include <thread>

#include "utils.cpp"
#include "windows.h"

void printRamInfo() {
    while (true) {
        cout << "Ram" << endl;
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