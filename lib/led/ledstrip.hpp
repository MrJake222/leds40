#pragma once

namespace mrwski {

class LEDStrip {

    const int pwm_max = (1<<10) - 1; // 10bit resolution

    const int pin_r;
    const int pin_g;
    const int pin_b;

    const int pins[3] = { pin_r, pin_g, pin_b };

public:
    LEDStrip(
        int pin_r_,
        int pin_g_,
        int pin_b_)
        : pin_r(pin_r_)
        , pin_g(pin_g_)
        , pin_b(pin_b_)
    { }

    void begin();
};

} // end namespace