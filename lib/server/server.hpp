#pragma once

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoJson.h>

#include <logger.hpp>
#include <wifimng.hpp>

namespace mrwski {

class Server {

    Logger& logger;
    WifiMng& wifimng;

    const short port;
    ESP8266WebServer esp_server;
    ESP8266HTTPUpdateServer esp_update_server;

    // server functions
    // TODO fix for authelia proxying
    // String get_client_ip() { return esp_server.client().remoteIP().toString(); }

    // JSON
    JsonDocument json_doc;
    static const int json_buf_size = 1024;
    char json_buf[json_buf_size];
    void serialize_and_send_json();

    void redirect(const String& url);

    void (*config_changed_callback)();
    void config_changed();

    void handle_wifi_scan();
    void handle_wifi_status();
    void handle_wifi_ap_config();
    void handle_wifi_sta_config();
    void handle_log();

    // int last_error = -1;
    // int led_state = 0;

    // helpers

public:
    Server(Logger& logger_,
           WifiMng& wifimng_,
           short port,
           void (*config_changed_callback)())
        : logger(logger_)
        , wifimng(wifimng_)
        , port(port)
        , esp_server(port)
        , config_changed_callback(config_changed_callback)
        {
            esp_update_server.setup(&esp_server);
        }

    void begin();
    void stop();
    void restart() { stop();
        begin(); }

    void loop();
};

} // end namespace mrjake