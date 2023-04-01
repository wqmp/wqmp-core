#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <SPI.h>
#include <WiFi.h>
#include <Arduino.h>

namespace network {
    /// @brief The line ending used in the HTTP format
    extern const String ENDL;

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

    /// @brief A struct to represent additional HTTP headers
    struct HttpHeader {
        /// @brief The header name
        String name;
        /// @brief The header value
        String value;
    };

    /**
     * @brief Setup WiFi connection
     * 
     * @param ssid Network name
     * @return true if connection worked
     */
    bool connect(const char* ssid);

    /**
     * @brief Setup WiFi connection with password
     * 
     * @param ssid Network name
     * @param pass Network password
     * @return true if connection worked
     */
    bool connect(const char* ssid, const char* pass);

    /// @brief Sends request and returns true if it worked
    bool send_http(const HttpRequest request);

    /// @brief Sends request with an additional header and returns true if it worked
    bool send_http(const HttpRequest request, const HttpHeader header);

    /// @brief Sends request and returns true if it worked
    bool send_https(const HttpRequest request);

    /// @brief Sends request with an additional header and returns true if it worked
    bool send_https(const HttpRequest request, const HttpHeader header);

    namespace {
        /// @brief Internal WifiClient instance
        WiFiClient client;

        /// @brief Sends request and returns true if it worked
        bool send_http(const HttpRequest request, const String extraHeaders) {
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
        bool send_http(const HttpRequest request, const String extraHeaders, const HttpHeader header)  {
            return send_http(request, extraHeaders + header.name + ": " + header.value + ENDL);
        }

        /// @brief Sends request and returns true if it worked
        template<typename... HttpHeaders>
        bool send_http(const HttpRequest request, const String extraHeaders, const HttpHeader header, HttpHeaders... args) {
            return send_http(request, String(extraHeaders + header.name + ": " + header.value + ENDL), args...);
        }

        WiFiSSLClient sslClient;

        /// @brief Sends request and returns true if it worked
        bool send_https(const HttpRequest request, const String extraHeaders) {
            if(sslClient.connect(request.host.c_str(), request.port)) {
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

                sslClient.print(text);

                return true;
            } else {
                return false;
            }        
        }

        /// @brief Sends request and returns true if it worked
        bool send_https(const HttpRequest request, const String extraHeaders, const HttpHeader header)  {
            return send_https(request, extraHeaders + header.name + ": " + header.value + ENDL);
        }

        /// @brief Sends request and returns true if it worked
        template<typename... HttpHeaders>
        bool send_https(const HttpRequest request, const String extraHeaders, const HttpHeader header, HttpHeaders... args) {
            return send_https(request, String(extraHeaders + header.name + ": " + header.value + ENDL), args...);
        }
    }

    /// @brief Sends request with a variable number of additional headers and returns true if it worked
    template<typename... HttpHeaders>
    bool send_http(const HttpRequest request, const HttpHeader header, const HttpHeaders... args) {
        String extraHeaders = header.name + ": " + header.value + ENDL;
        return send_http(request, extraHeaders, args...);
    }

    /// @brief Sends request with a variable number of additional headers and returns true if it worked
    template<typename... HttpHeaders>
    bool send_https(const HttpRequest request, const HttpHeader header, const HttpHeaders... args) {
        String extraHeaders = header.name + ": " + header.value + ENDL;
        return send_https(request, extraHeaders, args...);
    }

    /// @brief Get a string with WiFi debug information 
    String get_info();
}

#endif