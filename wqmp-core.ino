#include "headers/env.h"
#include "headers/pins.h"
#include "headers/utils.hpp"
#include "headers/network.hpp"
#include "headers/io.h"

#include <SPI.h>
#include <WiFi.h>

#ifndef ENDPOINT_PORT
#define ENDPOINT_PORT 80
#endif

#ifndef ALERT_PORT
#define ALERT_PORT 80
#endif

bool hasCriticalError = false;

/// @brief Do one-time setup work
void setup() {
  // Init Serial Connection (Development only)
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial is ready");
  Serial.println();

  // Init Network Connection
  if(network::connect(NETSSID
#ifdef NETPASS
  , NETPASS
#endif
  )) {
    Serial.println("Network is ready");
    Serial.println(network::get_info());
    Serial.println();
  } else {
    hasCriticalError = true;
    Serial.print("Network connection error");
    return;
  }

  // Setup light pin modes
  pinMode(PIN_STATUS_RED, OUTPUT);
  pinMode(PIN_STATUS_GREEN, OUTPUT);
  pinMode(PIN_STATUS_BLUE, OUTPUT);

  flowsensor::setup(PIN_FLOW);
}

template<typename T, typename F>
T take_median_reading(const size_t count, F sample, const int delay_ms = 20) {
  T values[count] = {};
  for(size_t i = 0; i < count; i++) {
    values[i] = sample();
    delay(delay_ms);
  }
  return median(count, values);
}

/// @brief Do repeated work
void loop() {
  if(hasCriticalError) return;

  double median_pH = take_median_reading<double>(10, get_pH);
  double median_TDS = take_median_reading<double>(10, get_TDS);
  double median_TBD = take_median_reading<double>(10, get_TBD);
  double median_fluoro = take_median_reading<double>(10, get_fluoro);
  
  
  unsigned int flow = flowsensor::get_flow();


  // if(pH < 5) {
  //   network::send_http(
  //     network::HttpRequest {
  //       "ntfy.sh", 80, "/water-alerts-mydevicenumer",
  //       "POST", "Water quality alert!", "text/plain"
  //     }
  //   );
  // }
// if(cyles)
//     String message = String("pH=")+pH+";flow=;tds=;";
//     network::send_http(
//         network::HttpRequest {
//           "ntfy.sh", 80, "/water-alerts-mydevicenumer",
//           "POST", message, "text/plain"
//         }
//     );
  // Do stuff over and over forever

  // aMax = 1023.0
  // vMax = 5.0
  // vIn = aIn * vMax / aMax

  // See screenshot
  // pH = vIn * -5.38421052632 + 28.8684210526

  // TBD (in NTU) = -370.8375 * vIn + 1382.5
  // clamp lower bound to 0
  
  // TDS = ?
  // Fluoro = ?

  // https://www.tutorialspoint.com/arduino/arduino_interrupts.htm
  // Flow = ?
  
  delay(100);
}