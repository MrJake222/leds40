#include "ledstatus.hpp"

#include <Arduino.h>

namespace mrwski {

void LEDStatus::on() {
    digitalWrite(pin, ON);
}

void LEDStatus::off() {
    digitalWrite(pin, OFF);
}

void LEDStatus::begin() {
    pinMode(pin, OUTPUT);
    blink = true;
    blink_last_ms = 0;
    blink_unit_cnt = 0;
    blink_on_units = 0;
}

void LEDStatus::loop() {
    int diff = (int)millis() - blink_last_ms;
    if (blink && (diff > blink_unit_ms)) {
        blink_unit_cnt += diff / blink_unit_ms;
        blink_unit_cnt %= blink_period_units;
        blink_last_ms = (int)millis();

        digitalWrite(pin, blink_unit_cnt < blink_on_units ? ON : OFF);
    }
}

} // end namespace