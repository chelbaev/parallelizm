#include <iostream>
#include <cmath>
#include <chrono>

#ifdef USE_DOUBLE
using ArrayType = double;
#else
using ArrayType = float;
#endif

int main() {
    const size_t size = 1e7;
    ArrayType* array = new ArrayType[size];

    // Заполнение массива значениями синуса
    for (size_t i = 0; i < size; ++i) {
        array[i] = std::sin(2 * M_PI * i / size);
    }

    // Вычисление суммы элементов массива
    ArrayType sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += array[i];
    }

    // Вывод суммы в терминал
    std::cout << "Sum: " << sum << std::endl;

    delete[] array;
    return 0;
}