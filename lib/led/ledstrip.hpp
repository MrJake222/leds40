#pragma once

#include <server.hpp>

namespace mrwski {

class LEDStrip {

    Server& server;

    const int pwm_max = (1<<8) - 1; // 8bit resolution
    const int pin_r;
    const int pin_g;
    const int pin_b;
    const int pins[3] = { pin_r, pin_g, pin_b };
    int values[3] = { 0, 0, 0 };

    void set_value(int i, int val);

    void handle_leds_get();
    void handle_leds_post();

public:
    LEDStrip(
        Server& server_,
        int pin_r_,
        int pin_g_,
        int pin_b_)
        : server(server_)
        , pin_r(pin_r_)
        , pin_g(pin_g_)
        , pin_b(pin_b_)
    { }

    void begin();
};

} // end namespace