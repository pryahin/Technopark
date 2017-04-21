// Copyright 2017 Vladimir Pryahin

/*
 ​Даны два массива целых чисел одинаковой длины A[0..n-1] и
 B[0..n-1].
 Необходимо найти первую пару индексов i0 и j0, i0 ≤ j0,
 такую что A[i0] + B[j0] = max {A[i] + B[j], где 0 <= i < n, 0 <= j < n, i <= j}.
 Время работы -O(n).
 */

/*==============================================
 Комментарий для преподавателя:
 ===============================================
 Используем 3 переменные. Собственно i0,j0, и еще заведем переменную для хранения 
 индекса максимального элемента А. Изначально все они будут ранвны 0
 
 Далее идем по массивам(1 цикл на оба массива). Если нынешний элемент А больше, 
 чем у нас сохранен, то переписываем индекс.
 Если сумма a[i0]+b[j0] меньше чем текущий элемент b[i] и максимальный найденный на данный
 момент элемент А a[IndexMaxA], то переписываем i0 на IndexMaxA, а j0 на i.
 
 Таким образом пройдя весь массив мы найдеим нужную на пару индексов.
 
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <utility>

std::pair<int, int> max_sum(int* a, int* b, int n) {
    unsigned int j0 = 0, i0 = 0;
    unsigned int IndexMaxA = 0;
    
    for (unsigned int i = 1; i < n; i++) {
        if (a[i] > a[IndexMaxA]) {
            IndexMaxA = i;
        }
        
        if ((a[IndexMaxA] + b[i]) > (a[i0] + b[j0])) {
            j0 = i;
            i0 = IndexMaxA;
        }
    }
    
    return std::make_pair(i0, j0);
}

int main() {
    unsigned int n;
    scanf("%u", &n);
    assert(n > 0);
    
    int* a = new int[n];
    int* b = new int[n];
    
    for (unsigned int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    for (unsigned int i = 0; i < n; i++) {
        scanf("%d", &b[i]);
    }
    
    std::pair<int, int> answer = max_sum(a, b, n);
    printf("%d %d", answer.first, answer.second);
    
    delete[] a;
    delete[] b;
    
    return 0;
}
