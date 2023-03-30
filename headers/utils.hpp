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

/// @brief Sorts an array in place
template<typename T>
void sort(const size_t size, T array[]) {
   for(size_t i = 0; i < size - 1; i++) {
        size_t mpos = i;
        for(size_t j = i+1; j < size; j++)
            if(array[j] < array[mpos])
                mpos = j;
        if (mpos != i) {
            T t = array[mpos];
            array[mpos] = array[i];
            array[i] = t;
        }
   }
}

/// @brief Takes the median of an array
template<typename T>
T median(const size_t size, const T array[]) {
    T sorted[size];
    memcpy(sorted, array, size*sizeof(T));
    sort(size, sorted);
    return sorted[(size-1)/2];
}
#endif
