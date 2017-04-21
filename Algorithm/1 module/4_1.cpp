// Copyright 2017 Vladimir Pryahin

/*
 В первой строке количество команд n. n ≤ 1000000.
 Каждая команда задаётся как 2 целых числа: a b.
 
 a = 2 - pop front
 a = 3 - push back
 
 Команды добавления элемента заданы с неотрицательным параметром b.
 Для очереди используются команды 2 и 3.
 Если дана команда pop*, то число b - ожидаемое значение.
 Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
 
 
 ​Очередь
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <cstring>

class queue {
private:
    int* buffer;  //Сам буфер
    size_t size, head, tail;     // size - размер, head - первый элемент, tail - последний
    void grow();  //Увеличение массива
public:
    queue();
    ~queue();
    void push_back(unsigned int);
    int pop_front();
};

queue::queue():size(4), head(0), tail(0), buffer(new int[4]()) {
}

queue::~queue() { delete[] buffer; }

void queue::push_back(unsigned int value) {
    buffer[head] = value;
    if (((head + 1) % size) == tail)
        grow();
    head = (++head) % size;
}

int queue::pop_front() {
    if (head == tail)
        return -1;
    int result = buffer[tail++];
    if (!(tail % size))
        tail = 0;
    return result;
}

void queue::grow() {
    int* tmp = new int[2 * size];
    memcpy(tmp, buffer, (head + 1) * sizeof(int));
    for (size_t i = 0; i<head+1; i++)
        tmp[i] = buffer[i];
    if (tail > head) {
        memcpy(tmp + size + tail, buffer + tail, (size - tail) * sizeof(int));
        tail += size;
    }
    
    size *= 2;
    delete[] buffer;
    buffer = tmp;
}

int main() {
    unsigned int n = 0;
    scanf("%u", &n);
    assert(n > 0);
    queue my_q;
    unsigned int command = 0;
    int element = 0;
    for (unsigned int i = 0; i < n; i++) {
        scanf("%u %d", &command, &element);
        assert(command == 3 || command == 2);
        if (command == 3)  //если команда 3, то вставляем, иначе достаем
            my_q.push_back(element);
        else if (!(element == my_q.pop_front())) {  //проверяем равен ли вытаскиваемый элемент ожидаемому
            printf("NO");
            return 0;
        }
    }
    printf("YES");  //Если все ожидания выполнены, то выводим YES и завершаем работу
    return 0;
}
