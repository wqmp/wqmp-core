
//https://reference.arduino.cc/reference/en/libraries/wifinina/wificlient/
#include "inc/env.h"
#include "inc/network.cpp"

#include <SPI.h>

#include <WiFi.h>
// #include <WiFiClient.h>
// #include <WiFiNINA.h>
// #include <WiFiServer.h>
// #include <WiFiSSLClient.h>
// #include <WiFiStorage.h>
// #include <WiFiUdp.h>

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
  // put your main code here, to run repeatedly:
}