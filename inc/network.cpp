//https://reference.arduino.cc/reference/en/libraries/wifinina/wificlient/
#include <SPI.h>
#include <WiFi.h>

namespace network {
    WiFiClient client;
    const String ENDL = "\r\n";

    /// @brief A struct to represent what is needed to send an HTTP request
    struct HttpRequest {
        /// @brief The target domain, e.g. "example.com"
        String host;
        /// @brief The port to try and connect to, e.g. 80
        int port;
        /// @brief The target route e.g. "/foo/bar/baz"
        String route;
        /// @brief The HTTP method to use, e.g. "POST", "GET", et al.
        String method;
        /// @brief The request content, e.g. "my message"
        String content;
        // The mime type of the content e.g. "application/x-www-form-urlencoded;" or "text/plain;"
        String contentType;
    };

    struct HttpHeader {
        String name;
        String value;
    };

    /**
     * @brief Setup WiFi connection
     * 
     * @param ssid Network name
     * @param pass Network password
     * @return true if connection worked
     */
    bool connect(const char* ssid, const char* pass) {
        return WiFi.begin(ssid, pass) == WL_CONNECTED;
    }

    namespace {
        /// @brief Sends request and returns true if it worked
        bool send_http(HttpRequest request, String extraHeaders) {
            if(client.connect(request.host.c_str(), request.port)) {
                String text =
                    request.method + " " + request.route + " HTTP/1.1" + ENDL
                    + "Host: " + request.host + ENDL
                    + "Connection: close" + ENDL
                    + "User-Agent: Arduino/1.0" + ENDL
                    + "Content-Type: " + request.contentType + ENDL
                    + "Content-Length: " + request.content.length() + ENDL
                    + extraHeaders
                    + ENDL
                    + request.content;
                ;

                client.print(text);

                return true;
            } else {
                return false;
            }        
        }
        /// @brief Sends request and returns true if it worked
        bool send_http(HttpRequest request, String extraHeaders, HttpHeader header) {
            return send_http(request, extraHeaders + header.name + ": " + header.value + ENDL);
        }

        /// @brief Sends request and returns true if it worked
        template<typename... HttpHeaders>
        bool send_http(HttpRequest request, String extraHeaders, HttpHeader header, HttpHeaders... args) {
            extraHeaders += header.name + ": " + header.value + ENDL;
            return send_http(request, extraHeaders, args...);
        }
    }
    
    /// @brief Sends request and returns true if it worked
    bool send_http(HttpRequest request) {
        return send_http(request, "");
    }

    /// @brief Sends request with an additional header and returns true if it worked
    bool send_http(HttpRequest request, HttpHeader header) {
        return send_http(request, "", header);
    }

    /// @brief Sends request with a variable number of additional headers and returns true if it worked
    template<typename... HttpHeaders>
    bool send_http(HttpRequest request, HttpHeader header, HttpHeaders... args) {
        String extraHeaders = header.name + ": " + header.value + ENDL;
        return send_http(request, extraHeaders, args...);
    }

    

    /// @brief Get a string with WiFi debug information 
    String get_info() {
        return String("SSID: ") + WiFi.SSID() + ", IP Address: " + WiFi.localIP() + ", Signal Strength (RSSI): " + WiFi.RSSI() + " dBm";
    }
}