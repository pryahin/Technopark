// Copyright 2017 Vladimir Pryahin

/*
 ​Дано натуральное число N. Представить N в виде A + B, так, что НОД(A,
 B)
 максимален, A ≤ B. ВывестиmA и B. Если возможно несколько ответов -
 вывести ответ с минимальным A. n ≤ 10^7
 */

#include <stdlib.h>
#include <stdio.h>
#include <cmath>

unsigned int mindiv(unsigned int n) {
    for (unsigned int i = 2; i * i < n; i++) {
        if (n % i == 0) return i;
    }
    return n;
}

int main() {
    unsigned int k;
    scanf("%u", &k);
    if (k > 1 && k < pow(10, 7)) {
        unsigned int divide = mindiv(k);
        printf("%d %d", k / divide, k - k / divide);
    }
    return 0;
}
