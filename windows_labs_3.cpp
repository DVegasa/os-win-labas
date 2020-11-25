#include <iostream>
#include <string>
#include <time.h>
#include <random>

#include "utils.cpp"
#include "windows.h"

void error(string msg) {
    cout << "Ошибка: " << msg << endl;
    exit(0);
}

int main(void) {
    srand(time(0));
    const int SIZE = 5;
    HANDLE processHeap = GetProcessHeap();
    if (processHeap == NULL) {
        error("processHeap");
    }

    for (int i = 0; i < SIZE; i++) {
        int length = rand() % 9 + 1;
        int* a = (int*)HeapAlloc(processHeap, 0, sizeof(int) * length);
        for (int i = 0; i < length; i++) {
            a[i] = rand() % 10;  // диапазон [0, 9] чтобы удобно выводить на консоль
        }

        cout << "#" << i << ": ";
        for (int i = 0; i < length; i++) {
            cout << a[i] << " ";
        }
        cout << endl;
    }

    return 0;
}