/*
 Дан массив целых чисел в диапазоне [0..109]. Размер массива кратен 10 и
 ограничен сверху значением 2.5 * 107 элементов. Все значения массива являются
 элементами псевдо-рандомной последовательности. Необходимо отсортировать
 элементы массива за минимально время и вывести каждый десятый элемент
 отсортированной последовательности.
 Реализуйте сортировку, основанную на QuickSort.
 Минимальный набор оптимизаций, который необходимо реализовать:
 1. Оптимизация ввода/вывода
 2. Оптимизация выбора опорного элемента
 3. Оптимизация Partition
 4. Оптимизация рекурсии
 5. Оптимизация концевой рекурсии
 */

#include <stack>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>

#define MAX_COUNT 25000000

//Рандомный пивот
size_t findPivot(int* target, size_t begin, size_t end) {
    begin = rand() % (end - begin + 1) + begin;
    size_t middle = rand() % (end - begin + 1) + begin;
    end = rand() % (end - begin + 1) + begin;
    
    if (target[middle] < target[begin]) {
        std::swap(middle, begin);
    }
    if (target[end] < target[begin]) {
        std::swap(end, begin);
    }
    if (target[end] < target[middle]) {
        std::swap(end, middle);
    }
    std::swap(target[middle], target[end]);
    return end;
}

size_t partition(int* target, size_t begin, size_t end) {
    //Случайный опорный элемент
    size_t pivotIndex = findPivot(target, begin, end);
    std::swap(target[begin], target[pivotIndex]);
    
    //итераторы
    size_t it = begin + 1, jt = end;
    
    // Hoare partition
    while (true) {
        while (target[it] < target[begin]) {
            ++it;
        }
        while (target[jt] > target[begin]) {
            --jt;
        }
        if (it < jt) {
            std::swap(target[it], target[jt]);
            ++it, --jt;
        } else {
            --it;
            std::swap(target[begin], target[it]);
            return it;
        }
    }
}

void insertionSort(int* target, size_t begin, size_t end) {
    for (size_t i = begin + 1; i < end; i++) {
        size_t left = 0, right = i;
        if (target[i] < target[i - 1]) {
            while (left < right) {
                size_t middle = left + (right - left) / 2;
                if (target[i] > target[middle]) {
                    left = middle + 1;
                } else {
                    right = middle;
                }
            }
            if (right < i - 1) {
                int temp = target[i];
                std::memmove(&target[right + 1], &target[right], (i - right) * sizeof(int));
                target[right] = temp;
            } else {
                std::swap(target[i], target[i - 1]);
            }
        }
    }
}

void qsort(int* target, size_t begin, size_t end) {
    //Стек пар begin, end
    std::stack<std::pair<int, int> > stack;
    stack.push(std::make_pair(begin, end));
    //Пока стек не пуст
    while (!stack.empty()) {
        std::pair<int, int> top = stack.top();
        begin = top.first;
        end = top.second;
        stack.pop();
        //Если элементов <= 1000, используем сортировку вставками
        if (end - begin <= 1000) {
            insertionSort(target + begin, 0, end - begin + 1);
            continue;
        }
        size_t pivot = partition(target, begin, end);
        if (pivot - 1 > begin) {
            stack.push(std::make_pair(begin, pivot - 1));
        }
        if (pivot + 1 < end) {
            stack.push(std::make_pair(pivot + 1, end));
        }
    }
}

int main() {
    int* array = new int[MAX_COUNT];
    size_t count = 0;
    while (scanf("%d", &array[count]) == 1) {
        count++;
    }
    srand(time(NULL));
    qsort(array, 0, count - 1);
    
    for (size_t i = 9; i < count; i += 10) {
        printf("%d ", array[i]);
    }
    
    delete[] array;
    return 0;
}
