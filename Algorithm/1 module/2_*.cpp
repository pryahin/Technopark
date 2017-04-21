/*
 Решение:
 Представим наш массив в виде ориентированного графа, где ячейки - вершины графа.
 Значение ячейки указывает в какую вершину ведет ребро
 
 В таком графе обязательно будет как минимум 1 цикл(обязательно есть повторяющийся элемент)
 Т.к. по условию массив заполнен числами из диапазона [0..n - 1), то логично будет начать проверку из последнего элемента.
 Заведем 2 переменные start и end. Собственно старт цикла и конец.
 Мы находим длину цикла. И идем с двух сторон по циклу, пока не прийдем в 1 и туvже точку.
 Эта точка будет нашим повтором.
 */

#include <stdlib.h>
#include <stdio.h>

int main() {
    int n = 0;
    scanf("%d", &n);
    
    int* array = (int*)calloc(n, sizeof(int));
    for (size_t i = 0; i < n; i++) scanf("%d", &array[i]);
    
    size_t end = array[n - 1];
    for (size_t i = 0; i < n; i++) end = array[end];
    
    size_t length = 1, start = end;
    
    end = array[end];
    while (start != end) {
        end = array[end];
        length++;
    }
    
    start = array[n - 1];
    end = array[n - 1];
    for (size_t i = 0; i < length; i++) end = array[end];
    
    while (start != end) {
        start = array[start];
        end = array[end];
    }
    
    printf("%zu", start);
    free(array);
    
    return 0;
}
