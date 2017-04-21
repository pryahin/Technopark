// Copyright 2017 Vladimir Pryahin

/*
 ​Дан массив целых чисел А[0..n-1]. Известно, что на интервале [0, m] значения
 массива строго возрастают, а на интервале [m, n-1] строго убывают.
 Найти m за O(log m).
 */

/*==============================================
 Комментарий для преподавателя:
 ===============================================
 Т.к. требуется найти за O(log m), то простой бинарный поиск не подходит, ибо он O(log n)
 В данном решении я сначала сужаю пространство поиска, и только потом ищу m.
 
 BinSearch -> собственно сам бинарный поиск
 Find -> сужает поиск(левую границу поиска квадратично), и вызывает 
 бинарный поиск на определенном пространстве.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

unsigned int BinSearch(int* a, unsigned int left, unsigned int right) {
    unsigned int mid = 0;
    while (right != left) {
        mid = left + (right - left) / 2;
        if (a[mid] < a[mid + 1]) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

unsigned int Find(int* a, unsigned int n) {
    unsigned int left = 1;
    while (a[left] > a[left - 1])
    {
        left <<=1;
        if (left>=n)
            break;
    }
    return BinSearch(a, left / 2, left>=n ? n-1 : left-1);
}

int main() {
    int n = 0;
    scanf("%d", &n);
    assert(n >= 2 && n <= 1000);
    int* a = new int[n];
    for (unsigned int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    printf("%u", Find(a, n));
    delete[] a;
    return 0;
}
