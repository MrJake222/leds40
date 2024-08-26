#pragma once

#include <espserver.hpp>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoJson.h>

#include <logger.hpp>
#include <wifimng.hpp>
#include "name.hpp"

namespace mrwski {

class Server {

    Logger& logger;
    WifiMng& wifimng;
    Name& name;

    const short port;
    // own wrapper class to expose lastChange protected field
    EspServer esp_server;
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
    const wl_status_t& sta_fail_reason;

    void handle_wifi_scan();
    void handle_wifi_status();
    void handle_wifi_ap_config();
    void handle_wifi_sta_config();
    void handle_log();
    void handle_name_get();
    void handle_name_post();

    void stop();

public:
    Server(Logger& logger_,
           WifiMng& wifimng_,
           Name& name_,
           short port,
           void (*config_changed_callback_)(),
           const wl_status_t& sta_fail_reason_)
        : logger(logger_)
        , wifimng(wifimng_)
        , name(name_)
        , port(port)
        , esp_server(port)
        , config_changed_callback(config_changed_callback_)
        , sta_fail_reason(sta_fail_reason_)
        {
            esp_update_server.setup(&esp_server);
        }

    void begin();
    void restart() { stop(); begin(); }
    void loop();

    ulong ms_since_last_conn();
};

} // end namespace mrjake