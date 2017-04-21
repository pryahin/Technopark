/*
 Дан массив строк. Количество строк не больше 100000. Отсортировать массив
 методом поразрядной сортировки MSD по символам. Размер алфавита - 256 символов.
 Последний символ строки = ‘0’.
 */

#include <iostream>
#include <cstring>
#include <vector>

#define ALPH 256

void counting_sort(char** a, int n, int k, int* count) {
    for (int i = 0; i < ALPH; ++i) {
        count[i] = 0;
    }
    for (int i = 0; i < n; ++i) {
        count[a[i][k]]++;
    }
    int sum = 0;
    for (int i = 0; i < ALPH; ++i) {
        int tmp = count[i];
        count[i] = sum;
        sum += tmp;
    }
    char** res = new char*[n];
    for (int i = 0; i < n; ++i) {
        res[count[a[i][k]]] = a[i];
        count[a[i][k]]++;
    }
    memcpy(a, res, n * sizeof(char*));
    
    delete[] res;
}

void msd_sort(char** a, int n, int k) {
    int* count = new int[ALPH + 1];
    count[ALPH] = n;
    counting_sort(a, n, k, count);
    for (int i = 0; i < ALPH; ++i) {
        int size = count[i + 1] - count[i];
        if (size > 1) {
            msd_sort(&a[count[i]], size, k + 1);
        }
    }
    delete[] count;
}

int main() {
    std::vector<char*> array;
    std::string str;
    while (std::cin >> str) {
        char* c_str = new char[str.length() + 1];
        strcpy(c_str, str.c_str());
        
        array.push_back(c_str);
    }
    msd_sort(&array[0], (int)array.size(), 0);
    for (int i = 0; i < array.size(); i++) std::cout << array[i] << std::endl;
    for (int i = 0; i < array.size(); i++) delete[] array[i];
    
    return 0;
}
