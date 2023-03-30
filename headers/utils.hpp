#ifndef UTILS_HPP
#define UTILS_HPP

#include <Arduino.h>

/// @brief Converts analog 0-1023 reading to 0.0-5.0V 
double a2v(const int analog);

/// @brief Takes the average of an array
template<typename T>
T average(const size_t size, const T array[]) {
    T sum = T(0);
    for(size_t i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum / size;
}
template<typename T>
void swap (T*a, T*b){
    T t = *a;
    *a = *b
    *b = t;
}


template<typename T>
T median (const size_t size, const T array[]) {
    T sorted[size] = {0};
    for(size_t i = 0; i < size; i++) {
        T min = array[1];
        for(size_t j = 1; j < size; j++)
    }
}
#endif