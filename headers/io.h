#ifndef IO_H
#define IO_H

double get_pH();

double get_TBD();

double get_TDS();

/// @brief Set status LED color
/// @param red 0-255 
/// @param green 0-255
/// @param blue 0-255
void set_LED(int red, int green, int blue);

namespace flowsensor {
  void setup(const int pin);
  unsigned int get_flow();
}

namespace fluorometer {
  struct Color {
    float red, green, blue;
  };
  bool setup();
  Color get_color();
  double get_fluoro();
}

#endif