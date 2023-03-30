#include "headers/env.h"
#include "headers/pins.h"
#include "headers/utils.hpp"
#include "headers/network.hpp"

#include <SPI.h>
#include <WiFi.h>

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
    Serial.print("Network connection error");
    return;
  }

  // Do other one-time setup work
  bool ok = network::send_http(
    network::HttpRequest {
      "ntfy.sh", 80, "/water-alerts-mydevicenumer",
      "POST", "Hello World!", "text/plain"
    }
  );
}

double get_pH() {
  int pinvalue = analogRead(PIN_PH);
  double voltage = a2v(pinvalue);
  double pH = voltage * -5.38421052632 + 28.8684210526;
  return pH;

}

int cycles = 0;
void loop() {
  cycles++;
  const int count = 10;
  double pHValues[count] = {};
  for(int i = 0; i < count; i++) {
    pHValues[i] = get_pH();
    delay(20);
  }
  double pH = average(count, pHValues);

  if(pH < 5) {
    network::send_http(
      network::HttpRequest {
        "ntfy.sh", 80, "/water-alerts-mydevicenumer",
        "POST", "Water quality alert!", "text/plain"
      }
    );
  }
if(cyles)
    String message = String("pH=")+pH+";flow=;tds=;";
network::send_http(
    network::HttpRequest {
      "ntfy.sh", 80, "/water-alerts-mydevicenumer",
      "POST", message, "text/plain"
    }
  );
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