#ifndef UTILS_H
#define UTILS_H

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

#endif