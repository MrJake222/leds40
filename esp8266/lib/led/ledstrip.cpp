#include "ledstrip.hpp"

#include <Arduino.h>

namespace mrwski {


void LEDStrip::begin() {
    analogWriteRange(pwm_max);

    for (int pin : pins) {
        pinMode(pin, OUTPUT);
        analogWrite(pin, 0);
    }

    server.func_route("/leds", HTTP_GET, [this] { handle_leds_get(); });
    server.func_route("/leds", HTTP_POST, [this] { handle_leds_post(); });
}

void LEDStrip::set_value(int i, int val) {
    analogWrite(pins[i], val*val / pwm_max);
    values[i] = val;
}

void LEDStrip::handle_leds_get() {
    server.json_doc.clear();
    server.json_doc["r"] = values[0];
    server.json_doc["g"] = values[1];
    server.json_doc["b"] = values[2];
    server.serialize_and_send_json();
}

void LEDStrip::handle_leds_post() {
    if (server.func_has_arg("r")) set_value(0, atoi(server.func_get_arg("r")));
    if (server.func_has_arg("g")) set_value(1, atoi(server.func_get_arg("g")));
    if (server.func_has_arg("b")) set_value(2, atoi(server.func_get_arg("b")));
    server.func_ack();
}

} // end namespace