#include <Arduino.h>

#define LED_STATUS  5
#define LED_R       12
#define LED_G       13
#define LED_B       14

#define PWM_MAX     1023

void setLed(int pin, int lvl) {
    analogWrite(pin, lvl*lvl/PWM_MAX);
}

void sweepUp(int p) {
    for (int i=0; i<=PWM_MAX; i++) {
        setLed(p, i);
        delay(1);
    }
}

void sweepDown(int p) {
    for (int i=PWM_MAX; i>=0; i--) {
        setLed(p, i);
        delay(1);
    }
}

void setup() {
    analogWriteRange(PWM_MAX);

    pinMode(LED_STATUS, OUTPUT);
    analogWrite(LED_STATUS, 0);

    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 0);

    sweepUp(LED_STATUS);
    sweepUp(LED_R);
}

void loop() {
    sweepUp(LED_G);
    sweepDown(LED_R);
    sweepUp(LED_B);
    sweepDown(LED_G);
    sweepUp(LED_R);
    sweepDown(LED_B);
}