#pragma once

namespace mrwski {

class LEDStatus {

    const int pin;
    const int ON = 1;
    const int OFF = 0;

    void on();
    void off();

    bool blink;
    int blink_last_ms;
    // state machine counts from 0 to 5
    // each step takes 20ms
    int blink_unit_cnt;
    const int blink_unit_ms = 25;
    const int blink_period_units = 10;
    int blink_on_units;

public:
    LEDStatus(
            int pin_)
            : pin(pin_)
    { }

    void begin();
    void loop();

    void ap_wait()  { /* short blinks */  blink_on_units = 1; }
    void ap_conn()  { /*      off     */  blink_on_units = 0; }
    void sta_wait() { /* long blinks  */  blink = true; blink_on_units = blink_period_units / 2; }
    void sta_ok()   { /*      off     */  blink_on_units = 0; }

    // this takes precedence over ap_* functions
    // can only be disabled by sta_wait()
    // (another connection is made)
    void sta_err()  { /*      on      */  blink = false; on(); }
};

}
