#include "headers/io.h"

#include "settings/settings.h"
#include "headers/utils.hpp"

#include <Wire.h>
#include <Adafruit_TCS34725.h>

#include "Arduino.h"

double get_pH() {
  return a2v(analogRead(PIN_PH)) * PH_SLOPE + PH_OFFSET;
}

double get_TBD() {
  return max(a2v(analogRead(PIN_TBD)) * TBD_SLOPE + TBD_OFFSET, 0);
}

double get_TDS() {
  return a2v(analogRead(PIN_TDS)) * TDS_SLOPE;
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
    unsigned int mL = L * 1000u;

    return mL;
  }
}



namespace fluorometer {
  namespace {
    Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
  }
  bool setup() {
    return tcs.begin(FLUORO_I2C);
  }
  Color get_color() {
    Color color;
    tcs.getRGB(&color.red, &color.green, &color.blue);
    return color;
  }
  double get_fluoro() {
    Color color = get_color();
    return
      (max(0, color.red - FLUORO_STD_RED) + max(0, color.green - FLUORO_STD_GREEN) + max(0, color.blue - FLUORO_STD_BLUE))/
      ((255 - FLUORO_STD_RED) + (255 - FLUORO_STD_GREEN) + (255 - FLUORO_STD_BLUE))
    ;
  }
}
