#include "headers/env.h"
#include "headers/pins.h"
#include "headers/utils.hpp"
#include "headers/network.hpp"
#include "headers/io.h"
#include "headers/thresholds.h"
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
T take_reading(const size_t count, F sample, const int delay_ms = 20) {
  T values[count] = {};
  for(size_t i = 0; i < count; i++) {
    values[i] = sample();
    delay(delay_ms);
  }
  return median(count, values);
}

bool send_alert(String Message = ""){
  return network::send_http(
    network::HttpRequest {
      ALERT_HOST, ALERT_PORT, ALERT_ROUTE,
      "POST", Message, "text/plain"
    }
  );
}
unsigned long last_report = 0, last_alert_reset = 0;
bool sent_pH_alert = false, sent_TDS_alert = false, sent_TBD_alert = false, sent_fluoro_alert = false;

/// @brief Do repeated work
void loop() {
  if(hasCriticalError) return;
  Serial.println(sent_pH_alert);

  double median_pH = 9.0; //take_reading<double>(10, get_pH);

  double median_TDS = take_reading<double>(10, get_TDS);
  double median_TBD = take_reading<double>(10, get_TBD);
  double median_fluoro = take_reading<double>(10, get_fluoro);
  
  
  if(median_pH>SAFE_PH_MAX && !sent_pH_alert) sent_pH_alert = send_alert("pH is too high!");
  if(median_pH<SAFE_PH_MIN && !sent_pH_alert) sent_pH_alert = send_alert("pH is too low!");
  if(median_TDS>SAFE_TDS_MAX && !sent_TDS_alert) sent_TDS_alert = send_alert("TDS is too high!");
  if(median_TBD>SAFE_TBD_MAX && !sent_TBD_alert) sent_TBD_alert = send_alert("TBD is too high!");
  if(median_fluoro>SAFE_FLUORO_MAX && !sent_fluoro_alert) sent_fluoro_alert = send_alert("Fluorescence is too high!");

  unsigned long now = millis ();
  if(now-last_report>= 1000*60*30||last_report==0){
  unsigned int flow = flowsensor::get_flow();
    last_report = now;
    Serial.print ("Report sent");
      String data = String ("")+"ph="+median_pH+";tds="+median_TDS+";tbd="+median_TBD+";fluoro="+median_fluoro+";flow="+flow;
      network::send_http(
        network::HttpRequest {
          ENDPOINT_HOST, ENDPOINT_PORT, ENDPOINT_ROUTE,
          "POST", data, "text/plain"
        }
      );
    if(now-last_alert_reset>=1000*60*60*12){
      last_alert_reset = now;
      sent_pH_alert = sent_TDS_alert = sent_TBD_alert = sent_fluoro_alert = false;
      
    }
  }
  
  



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
