
//https://reference.arduino.cc/reference/en/libraries/wifinina/wificlient/
#include <SPI.h>

#include <WiFi.h>
// #include <WiFiClient.h>
// #include <WiFiNINA.h>
// #include <WiFiServer.h>
// #include <WiFiSSLClient.h>
// #include <WiFiStorage.h>
// #include <WiFiUdp.h>

WiFiClient client;
char ssid[] = ""
  #include ".env/ssid.txt"
;
char pass[] = ""
  #include ".env/netpass.txt"
;
#define ENDL "\r\n"

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial is ready");
  Serial.println();

  Serial.print("SSID: '");
  Serial.print(ssid);
  Serial.println("'");
  Serial.println("Attempting wifi connection");

  int status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    Serial.println("Stopping!");
    return;
  } else {
    Serial.println("Got wifi connection");
  }

  if(client.connect("ntfy.sh", 80)) {
    Serial.println(
      "POST /IKLyZfnXFjUrFXYq HTTP/1.1" ENDL
      "Host: ntfy.sh" ENDL
      "Title: CustomTitle" ENDL
      ENDL
      "It works"
    );
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}