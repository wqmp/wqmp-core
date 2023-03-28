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
  network::send_http(
    network::HttpRequest {
      "ntfy.sh", 80, "/IKLyZfnXFjUrFXYq",
      "POST", "Hello World!", "text/plain"
    }, 
    network::HttpHeader {"Title", "Wow!"},
    network::HttpHeader {"Priority", "5"},
    network::HttpHeader {"Tags", "unicorn"}
  );
}

void loop() {
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