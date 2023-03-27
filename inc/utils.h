#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

/// @brief Converts analog 0-1023 reading to 0.0-5.0V 
double a2v(const int analog);

#endif