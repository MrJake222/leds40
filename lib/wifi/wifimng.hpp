#pragma once

#include <Arduino.h>

#include <logger.hpp>

namespace mrwski {

class WifiMng {
    Logger& logger;

    int last_check_ms;

    bool ap;
    int ap_time;
    const int ap_time_max = 60;
    void ap_loop();

    bool sta_check;
    int sta_tries;
    void sta_loop();

public:
    WifiMng(Logger& logger_)
        : logger(logger_)
        { }

    void begin();
    void loop();
    void wait_for_sta();

    struct wifi_mode {
        bool ap;
        bool sta;
        String name;
    };
    static struct wifi_mode mode_to_struct(int code);
    static String sta_status_to_string(int status);

};

} // end namespace
