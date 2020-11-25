#include <time.h>

#include <iostream>
#include <random>
#include <string>

#include "utils.cpp"
#include "windows.h"

void error(string msg) {
    cout << "Ошибка: " << msg << endl;
    exit(0);
}

int length(HANDLE heap, int* p) {
    int size = HeapSize(heap, 0, p);
    if (size == (SIZE_T)-1) error("HeapSize");
    return (size / sizeof(int));
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
        if (a == NULL) error("HeapAlloc at " + i);

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
        cout << endl
             << endl;
    }

    // Вывод на экран размеров массивов
    cout << "Размеры" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << "#" << i << ": " << length(processHeap, ar[i]) << endl;
    }

    // Сортировка
    for (int i = 0; i < SIZE; i++) {
        for (int j = SIZE - 1; j > i; j--) {
            if (length(processHeap, ar[j - 1]) > length(processHeap, ar[j])) {
                // Поменять местами [j-1] и [j]

                // Буфер для обмена
                int* t = (int*)HeapAlloc(processHeap, 0, sizeof(int) * length(processHeap, ar[j - 1]));
                if (t == NULL) error("HeapAlloc");
                for (int k = 0; k < length(processHeap, ar[j - 1]); k++) {
                    // Копипастим значения в буфер
                    t[k] = ar[j - 1][k];
                }

                // Уменьшаем размер массива
                ar[j - 1] = (int*)HeapReAlloc(processHeap, 0, ar[j - 1], sizeof(int) * length(processHeap, ar[j]));
                if (ar[j - 1] == NULL) error("HeapReAlloc");
                for (int k = 0; k < length(processHeap, ar[j]); k++) {
                    // Копипастим значения в урезанный массив
                    ar[j - 1][k] = ar[j][k];
                }

                // Урезаем в длине другой массив
                ar[j] = (int*)HeapReAlloc(processHeap, 0, ar[j], sizeof(int) * length(processHeap, t));
                if (ar[j] == NULL) error("HeapReAlloc");
                for (int k = 0; k < length(processHeap, t); k++) {
                    // Копипастим значения из буфера в массив
                    ar[j][k] = t[k];
                }
            }
        }
    }

    // Выводим отсортированный
    cout << "Отсортированный:" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << " a#" << i << ": ";
        for (int j = 0; j < length(processHeap, ar[i]); j++) {
            cout << ar[i][j] << " ";
        }
        cout << endl;
    }

    // Освобождаем память
    for (int i = 0; i < SIZE; i++) {
        if (HeapFree(processHeap, 0, ar[i]) == 0) {
            error("HeapFree: " + GetLastError());
        } else {
            cout << "Память освобождена" << endl;
        }
    }
    return 0;
}