#include "ledstrip.hpp"

#include <Arduino.h>

namespace mrwski {


void LEDStrip::begin() {
    analogWriteResolution(pwm_max);

    for (auto pin : pins) {
        pinMode(pin, OUTPUT);
        analogWrite(pin, 0);
    }
}

} // end namespace