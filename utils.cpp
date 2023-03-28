#include "headers/utils.hpp"

/// @brief Converts analog 0-1023 reading to 0.0-5.0V 
double a2v(const int analog) {
    return analog * 5.0 / 1023.0;
}