/*
 На вокзале есть некоторое количество тупиков, куда прибывают электрички. Этот вокзал является
 их конечной станцией. Дано расписание движения электричек, в котором для каждой электрички
 указано время ее прибытия, а также время отправления в следующий рейс. Электрички в
 расписании упорядочены по времени прибытия. Когда электричка прибывает, ее ставят в
 свободный тупик с минимальным номером. При этом если электричка из какого-то тупика
 отправилась в момент времени X, то электричку, которая прибывает в момент времени X, в этот
 тупик ставить нельзя, а электричку, прибывающую в момент X+1 — можно.
 В данный момент на вокзале достаточное количество тупиков для работы по расписанию.
 Напишите программу, которая по данному расписанию определяет, какое минимальное
 количество тупиков требуется для работы вокзала.
 */

#include <iostream>
#include <cassert>

class Heap {
 private:
  void SiftDown(int i);
  void SiftUp(int i);

  void grow();

  int* buffer;
  int size;
  int capacity;

 public:
  Heap(int cap = 10);
  ~Heap();

  int ExtractMin();
  void add(int element);

  inline bool IsEmpty() const { return size == 0; }
  inline int GetSize() const { return size; }
  inline int PeekAtNext() const { 
    assert(!IsEmpty());
    return buffer[0]; 
  }
};

Heap::Heap(int cap) {
  capacity = cap;
  size = 0;
  buffer = new int[capacity];
}

Heap::~Heap() { 
  delete[] buffer; 
}

// Проталкивание элемента вниз.
void Heap::SiftDown(int i) {
  int left = 2 * i + 1;
  int right = 2 * i + 2;
  //Ищем меньшего сына, если такой есть
  int smallest = i;
  if (left < size && buffer[left] < buffer[i]) smallest = left;

  if (right < size && buffer[right] < buffer[smallest]) smallest = right;
  //Если меньший сын есть, то проталкиваем корень в него
  if (smallest != i) {
    std::swap(buffer[i], buffer[smallest]);
    SiftDown(smallest);
  }
}

//Проталкивание элемента наверх
void Heap::SiftUp(int index) {
  while (index > 0) {
    int parent = (index - 1) / 2;
    if (buffer[index] >= buffer[parent]) return;
    std::swap(buffer[index], buffer[parent]);
    index = parent;
  }
}

void Heap::grow() {
  int new_capacity = capacity * 2;
  int* new_buffer = new int[new_capacity];

  for (int i = 0; i < capacity; i++) new_buffer[i] = buffer[i];

  delete[] buffer;

  buffer = new_buffer;
  capacity = new_capacity;
}

void Heap::add(int element) {
  buffer[size] = element;
  SiftUp(size++);

  if (size == capacity) grow();
}

int Heap::ExtractMin() {
  if (IsEmpty()) 
    return 0;

  int result = buffer[0];

  buffer[0] = buffer[--size];
  SiftDown(0);

  return result;
}

// Подсчет минимально необходимого количества тупиков
int GetMinCount(int n) {
  int minCount = 0;
  int arrival = 0, departure = 0;
  Heap heap;

  for (int i = 0; i < n; i++) {
    // Ввод времени прибытия и отбытия
    std::cin >> arrival >> departure;

    if (!heap.IsEmpty()) {
      // Если некоторые электрички к моменту прибытия уехали - извлекаем
      while (arrival > heap.PeekAtNext() && !heap.IsEmpty()) {
        heap.ExtractMin();
      }
    }

    // Добавляем время отправления прибывшей
    heap.add(departure);

    // Смотрим, сколько путей занято
    if (heap.GetSize() > minCount) minCount = heap.GetSize();
  }

  return minCount;
}

int main() {
  int n = 0;

  std::cin >> n;
  std::cout << GetMinCount(n);

  return 0;
}
