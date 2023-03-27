#include "inc/env.h"
#include "inc/network.cpp"
#include "inc/utils.h"
#include "inc/pins.h"

#include <SPI.h>
#include <WiFi.h>


void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial is ready");
  Serial.println();

  if(network::connect(NETSSID, NETPASS)) {
    Serial.println("Network is ready");
    Serial.println(network::get_info());
    Serial.println();
  } else {
    Serial.print("Network connection error");
    return;
  }

  network::send_http(
    network::HttpRequest {
      "ntfy.sh", 80, "/IKLyZfnXFjUrFXYq",
      "POST", "Hello World", "text/plain"
    }, 
    network::HttpHeader {"Title", "Wow!"},
    network::HttpHeader {"Priority", "5"}
  );

}

void loop() {
  // aMax = 1023.0
  // vMax = 5.0
  // vIn = aIn * vMax / aMax

  // See screenshot
  // pH = vIn * -5.38421052632 + 28.8684210526

  // TBD (in NTU) = -370.8375 * vIn + 1382.5
  // clamp lower bound to 0
  
  // TDS = ?
  // Fluoro = ?
  // Flow = ?
  
  delay(100);
}