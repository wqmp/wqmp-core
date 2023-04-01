//https://reference.arduino.cc/reference/en/libraries/wifinina/wificlient/
#include <SPI.h>
#include <WiFi.h>
#include "headers/network.hpp"

using namespace network;

namespace network {

    /// @brief The line ending used in the HTTP format
    const String ENDL = "\r\n";

    /**
     * @brief Setup WiFi connection
     * 
     * @param ssid Network name
     * @return true if connection worked
     */
    bool connect(const char* ssid) {
        return WiFi.begin(ssid) == WL_CONNECTED;
    }

    /**
     * @brief Setup WiFi connection with password
     * 
     * @param ssid Network name
     * @param pass Network password
     * @return true if connection worked
     */
    bool connect(const char* ssid, const char* pass) {
        return WiFi.begin(ssid, pass) == WL_CONNECTED;
    }
    
    /// @brief Sends request and returns true if it worked
    bool send_http(const HttpRequest request) {
        return send_http(request, "");
    }

    /// @brief Sends request with an additional header and returns true if it worked
    bool send_http(const HttpRequest request, const HttpHeader header) {
        return send_http(request, "", header);
    }

    /// @brief Sends request and returns true if it worked
    bool send_https(const HttpRequest request) {
        return send_https(request, "");
    }

    /// @brief Sends request with an additional header and returns true if it worked
    bool send_https(const HttpRequest request, const HttpHeader header) {
        return send_https(request, "", header);
    }

    /// @brief Get a string with WiFi debug information 
    String get_info() {
        return String("SSID: ") + WiFi.SSID() + ", IP Address: " + WiFi.localIP() + ", Signal Strength (RSSI): " + WiFi.RSSI() + " dBm";
    }
}