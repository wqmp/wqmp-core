#include "settings/settings.h"
#include "headers/utils.hpp"
#include "headers/network.hpp"
#include "headers/io.h"

#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>


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

bool send_alert(String message = "") {
  Serial.println("Sending alert!");
  return network::send_http(
    network::HttpRequest {
      ALERT_HOST, ALERT_PORT, ALERT_ROUTE,
      "POST", message, "text/plain"
    }
  );
}
unsigned long last_report = 0, last_alert_reset = 0;
bool sent_pH_alert = false, sent_TDS_alert = false, sent_TBD_alert = false, sent_fluoro_alert = false;

/// @brief Do repeated work
void loop() {
  if(hasCriticalError) return;

  const double median_pH = take_reading<double>(SAMPLE_SIZE, get_pH);
  const double median_TDS = take_reading<double>(SAMPLE_SIZE, get_TDS);
  const double median_TBD = take_reading<double>(SAMPLE_SIZE, get_TBD);
  const double median_fluoro = take_reading<double>(SAMPLE_SIZE, get_fluoro);
  
  
  if(median_pH>SAFE_PH_MAX && !sent_pH_alert) sent_pH_alert = send_alert("pH is too high!");
  if(median_pH<SAFE_PH_MIN && !sent_pH_alert) sent_pH_alert = send_alert("pH is too low!");
  if(median_TDS>SAFE_TDS_MAX && !sent_TDS_alert) sent_TDS_alert = send_alert("TDS is too high!");
  if(median_TBD>SAFE_TBD_MAX && !sent_TBD_alert) sent_TBD_alert = send_alert("TBD is too high!");
  if(median_fluoro>SAFE_FLUORO_MAX && !sent_fluoro_alert) sent_fluoro_alert = send_alert("Fluorescence is too high!");

  const unsigned long now = millis ();

  if(now-last_report >= 1uL*REPORT_INTERVAL || last_report == 0) {
    const unsigned int flow = flowsensor::get_flow();
    last_report = now;
    Serial.println("Report sent");
    String data = String ("")+"ph="+median_pH+";tds="+median_TDS+";tbd="+median_TBD+";fluoro="+median_fluoro+";flow="+flow;
    network::send_https(
      network::HttpRequest {
        ENDPOINT_HOST, ENDPOINT_PORT, ENDPOINT_ROUTE,
        "POST", data, "text/plain"
      }
    );
  }

  if(now-last_alert_reset >= 1uL*ALERT_COOLDOWN_INTERVAL) {
    Serial.println("New alert cycle started");
    last_alert_reset = now;
    sent_pH_alert = sent_TDS_alert = sent_TBD_alert = sent_fluoro_alert = false;
  }

  delay(1000);
}
