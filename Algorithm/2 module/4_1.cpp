/*
​Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition
реализуйте методом прохода двумя итераторами от начала массива к концу.
*/

#include <iostream>

int median(int* data, size_t begin, size_t end) {
  if (data[begin] < data[(begin + end) / 2]) {
    std::swap(data[begin], data[end]);
  }
  if (data[begin] < data[end]) {
    std::swap(data[begin], data[end]);
  }
  if (data[(begin + end) / 2] > data[end]) {
    std::swap(data[(begin + end) / 2], data[end]);
  }
  return data[end];
}

size_t Partition(int* data, size_t begin, size_t end) {
  int pivot = median(data, begin, end);
  size_t i = begin, j = begin;
  while (j < end) {
    if (data[j] <= pivot) {
      std::swap(data[i++], data[j++]);
    } else {
      j++;
    }
  }
  std::swap(data[i], data[end]);
  return i;
}

int kstat(int* data, size_t n, size_t k) {
  size_t begin = 0, end = n - 1;
  while (true) {
    size_t part = Partition(data, begin, end);
    if (part == k) {
      return data[part];
    }
    if (part > k)
      end = part - 1;
    else
      begin = part + 1;
  }

  return 0;
}

int main() {
  size_t n = 0, k = 0;

  std::cin >> n >> k;
  int* data = new int[n];
  for (size_t i = 0; i < n; i++) {
    std::cin >> data[i];
  }

  std::cout << kstat(data, n, k);
  delete[] data;
  return 0;
}
