#include "headers/io.h"

#include "headers/pins.h"
#include "headers/utils.hpp"

#include "Arduino.h"

// Linear Fit: https://www.desmos.com/calculator/29irnoch4x

double get_pH() {
  return a2v(analogRead(PIN_PH)) * -5.38421052632 + 28.8684210526;
}

double get_TBD() {
  return max(a2v(analogRead(PIN_TBD)) * -370.8375 + 1382.5, 0);
}

double get_TDS() {
  return a2v(analogRead(PIN_TDS)) * 8.92857142857;
}

double get_fluoro() {
  #pragma GCC warn Not Finished
  return 0;
}

/// @brief Set status LED color
/// @param red 0-255
/// @param green 0-255
/// @param blue 0-255
void set_LED(int red, int green, int blue) {
  analogWrite(PIN_STATUS_RED, red);
  analogWrite(PIN_STATUS_GREEN, green);
  analogWrite(PIN_STATUS_BLUE, blue);
}


namespace flowsensor {
  const double FLOW_FREQUENCY = 4.8;// Q (L/min)
  namespace {
    unsigned long last_read;
    volatile int flow_pulses = 0;
    void on_pulse() {
      flow_pulses++;
    }
  }

  void setup(const int pin) {
    attachInterrupt(digitalPinToInterrupt(pin), on_pulse, RISING);
    last_read = millis();
    flow_pulses = 0;
  }

  unsigned int get_flow() {
    const unsigned long now = millis(), pulses = flow_pulses;
    flow_pulses = 0;
    const double deltaTs = (now - last_read) / 1000.0; // seconds
    
    last_read = now;

    double L = pulses / FLOW_FREQUENCY * (deltaTs/60.0); // n * (L/min) * min = L
    unsigned int mL = L * 1000;

    return mL;
  }
}
