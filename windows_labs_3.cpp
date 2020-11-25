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

int length(HANDLE heap, int* p) {
    int size = HeapSize(heap, 0, p);
    return (size / sizeof(int)) ;
}

int main(void) {
    srand(time(0));
    HANDLE processHeap = GetProcessHeap();
    if (processHeap == NULL) {
        error("processHeap");
    }

    const int SIZE = 5;
    int* ar[SIZE];
    // Генерация исходных данных
    for (int i = 0; i < SIZE; i++) {
        int length = rand() % 15 + 1; 
        int* a = (int*)HeapAlloc(processHeap, 0, sizeof(int) * length);
        for (int j = 0; j < length; j++) {
            a[j] = rand() % 10;  // диапазон [0, 9] чтобы удобно выводить на консоль
        }
        ar[i] = a;

        // Вывод на экран полученный массива
        cout << " a#" << i << ": ";
        for (int j = 0; j < length; j++) {
            cout << a[j] << " ";
        }
        cout << endl;

        // Вывод того же массива, но с указателя ar
        cout << "ar#" << i << ": ";
        for (int j = 0; j < length; j++) {
            cout << ar[i][j] << " ";
        }
        cout << endl << endl;
    }

    // Вывод на экран размеров массивов
    cout << "Размеры" << endl;
    for (int i = 0; i < SIZE; i++) {
        // int size = HeapSize(processHeap, 0, ar[i]);
        // cout << "#" << i << ": " << (size / sizeof(int)) << endl;
        cout << "#" << i << ": " << length(processHeap, ar[i]) << endl;
    }

    // Сортировка
    // for (int i = 0; i < SIZE; i++) {
    //     for (int j = SIZE-1; j > i; j--) {
    //         if ()
    //     }
    // }
    return 0;   
}