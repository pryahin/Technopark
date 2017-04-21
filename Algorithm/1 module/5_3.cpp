// Copyright 2017 Vladimir Pryahin

/*
 Дана последовательность N прямоугольников различной ширины и высоты (wi,hi).
 Прямоугольники расположены, начиная с точки (0, 0), на оси ОХ вплотную друг за
 другом (вправо).
 Требуется найти M - площадь максимального прямоугольника (параллельного осям
 координат),
 который можно вырезать из этой фигуры.
 */

#include <iostream>
#include <assert.h>

struct Rectangle {
 public:
  int width;
  int height;
  int X;
  Rectangle() : width(0), height(0), X(0) {}
  Rectangle(int _width, int _height, int _X)
      : width(_width), height(_height), X(_X) {}
  ~Rectangle() {}
  inline int Square() { return width * height; }
};

class Stack {
 private:
  Rectangle* stackPtr;  // указатель на стек
  int size;             // максимальное количество элементов в стеке
  int top;  // номер текущего элемента стека
  inline void grow();

 public:
  Stack();
  ~Stack();
  inline void Push(const Rectangle&);  // поместить элемент в вершину стека
  inline Rectangle Pop();       // удалить элемент из вершины стека и вернуть его
  inline Rectangle& Peek(int);  // n-й элемент от вершины стека
  inline int GetTop() const;  // получить номер текущего элемента в стеке
};

inline void Stack::grow() {
  Rectangle* temp = new Rectangle[size + 10];
  for (size_t i = 0; i < size; i++) temp[i] = stackPtr[i];
  size += 10;
  delete[] stackPtr;
  stackPtr = temp;
}
Stack::Stack() : size(10), top(0) { stackPtr = new Rectangle[10]; }
Stack::~Stack() { delete[] stackPtr; }
inline void Stack::Push(const Rectangle& value) {
  if (top == size) grow();
  stackPtr[top++] = value;
}
inline Rectangle Stack::Pop() { return stackPtr[--top]; }
inline Rectangle& Stack::Peek(int n) { return stackPtr[top - n]; }
inline int Stack::GetTop() const { return top; }

int GetMaxSquare(Rectangle* array,
                 int n) {  //Функция получения максимального подпррямоугольника
  Stack stack;
  int maxSquare = 0;
  for (int i = 0; i < n; i++) {
    if (stack.GetTop() == 0)  //Если стек пуст, то вставляем прямоугольник в
                              //стек и меняем при необходимости макс. площаль
    {
      array[i].X = 0;
      if (array[i].Square() > maxSquare) maxSquare = array[i].Square();
      stack.Push(array[i]);
      continue;
    } else {
      array[i].X = stack.Peek(1).X +
                   stack.Peek(1).width;  //Если не пуст, то меняем координату
    }

    if (array[i].height >
        stack.Peek(1).height)  //Если высота больше, то без проблем добавляем
    {
      stack.Push(array[i]);
    } else {                         //Если же ниже :c
      Rectangle last = stack.Pop();  //Вытаскиваем прошлый прямоугольник
      int endX =
          array[i].X + array[i].width;  //Получаем конец текущего прямоугольника
      int lastEndX =
          last.X + last.width;  //Получаем конец прошлого прямоугольника

      if (last.Square() > maxSquare) {  //Площадь больше => меняем значение
        maxSquare = last.Square();
      }

      while (stack.GetTop() != 0 &&
             stack.Peek(1).height >=
                 array[i].height) {  //Пока стек не пуст и высота посоледнего
                                     //элемента в стеке >= текущей
        last = stack.Pop();  //Вытаскиваем последний элемент
        int lastSquare = (lastEndX - last.X) *
                         last.height;  //Площадь (под)прямоугольника. Высота =
                                       //тому, что сейчас вытащили, а вот ширина
                                       //равна концу прямоугольник, который
                                       //вытащили до цикла - его координата Х.
                                       //Таким образом получаем площади
                                       //вложенных прямоугольников.
        if (lastSquare > maxSquare) {  //Площадь больше => меняем
          maxSquare = lastSquare;
        }
      }
      stack.Push(Rectangle(endX - last.X, array[i].height,
                           last.X));  //Вставляем новый прямоугольник
    }
  }

  return maxSquare;
}

int main() {
  int n = 0;
  assert(scanf("%d", &n) == 1);
  Rectangle* array = new Rectangle[n + 1]();
  for (int i = 0; i < n; i++) {
    int width, height;
    if (scanf("%d %d", &width, &height) == 2) {
      array[i].width = width;
      array[i].height = height;
    }
  }
  printf("%d", GetMaxSquare(array, n + 1));
  delete[] array;

  return 0;
}
