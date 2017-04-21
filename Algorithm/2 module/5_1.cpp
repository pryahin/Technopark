/*
Дана очень длинная последовательность целых чисел длины n. 
Требуется вывести в отсортированном виде её первые k элементов. 
Последовательность может не помещаться в память. Время работы O(n * log(k)). Память O(k). 
Использовать слияние.
*/

#include <iostream>
#include <vector>

using data = std::vector<int>;

// Сливаем две части [left, middle] и [middle+1, right]
void Merge(data& array, size_t left, size_t middle, size_t right) {
  data buffer(right - left + 1);
  size_t leftPosition = left;
  size_t rightPosition = middle + 1;
  size_t bufferPosition = 0;

  // Пока одна из частей не достигла конца конца.
  while (leftPosition <= middle && rightPosition <= right) {
    if (array[leftPosition] <= array[rightPosition])
      buffer[bufferPosition++] = array[leftPosition++];
    else
      buffer[bufferPosition++] = array[rightPosition++];
  }

  // Если осталось, дописываем часть
  while (leftPosition <= middle)
    buffer[bufferPosition++] = array[leftPosition++];

  while (rightPosition <= right)
    buffer[bufferPosition++] = array[rightPosition++];

  std::copy(buffer.begin(), buffer.end(), array.begin() + left);
}

void MergeSort(data& array, size_t left, size_t right) {
  if (left == right) return;

  size_t middle = (left + right) / 2;

  MergeSort(array, left, middle);  // Сортировка левой части
  MergeSort(array, middle + 1, right);  // Сортировка правой части
  Merge(array, left, middle, right);  // Сливаем
}

//Слияние двух массивов до к элементов
void MergeForK(data& first, data& second, size_t k, size_t i) {
  data buffer(k);
  int firstPosition = 0;
  int secondPosition = 0;
  int bufferPosition = 0;

  // Слияние k элементов, или i , если элементов в two меньше k
  while (firstPosition < k && secondPosition < i && bufferPosition < k) {
    if (first[firstPosition] <= second[secondPosition])
      buffer[bufferPosition++] = first[firstPosition++];
    else
      buffer[bufferPosition++] = second[secondPosition++];
  }

  // Если в two i элементов, дописываем в буфер из one
  while (firstPosition < k && bufferPosition < k)
    buffer[bufferPosition++] = first[firstPosition++];

  // Копируем буфер в исходный вектор с границами
  std::copy(buffer.begin(), buffer.end(), first.begin());
}

int main() {
  int64_t n = 0;
  size_t k = 0;
  std::cin >> n >> k;

  data result(k), buffer(k);
  size_t i = 0;

  for (; i < k; i++) std::cin >> result[i];

  MergeSort(result, 0, k - 1);

  int64_t j = k;
  while (j < n) {
    for (i = 0; i < k && j < n; i++, j++) std::cin >> buffer[i];

    MergeSort(buffer, 0, i - 1);

    MergeForK(result, buffer, k, i);
  }

  for (auto out : result) {
    std::cout << out << " ";
  }

  return 0;
}
