#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <logger.hpp>
#include <ledstatus.hpp>

namespace mrwski {

class WifiMng {

    // ticks per second
    const int tps = 1;
    int last_check_ms;

    Logger& logger;
    LEDStatus& ledst;
    void (*config_changed_callback)();

    bool ap;
    int ap_time;
    const int ap_time_max = 60 * tps;
    void ap_loop();

    bool sta_check;
    int sta_time;
    bool sta_fail;
    const int sta_fail_time = 60 * tps;
    wl_status_t& sta_fail_reason;
    void sta_loop();


public:
    WifiMng(Logger& logger_,
            LEDStatus& ledst_,
            void (*config_changed_callback_)(),
            wl_status_t& sta_fail_reason_)
        : logger(logger_)
        , ledst(ledst_)
        , config_changed_callback(config_changed_callback_)
        , sta_fail_reason(sta_fail_reason_)
    { }

    void begin();
    void loop();
    void wait_for_sta(bool fail);

    struct wifi_mode {
        bool ap;
        bool sta;
        const char* name;
    };
    static struct wifi_mode mode_to_struct(WiFiMode_t code);
    static const char* status_to_string(wl_status_t status);
};

} // end namespace
