#include <Arduino.h>

#include <hw.hpp>
#include <logger.hpp>
#include <wifimng.hpp>
#include <wifievents.hpp>
#include <server.hpp>
#include <ledstrip.hpp>

mrwski::Logger logger;
mrwski::WifiMng wifimng(logger);
mrwski::WifiEvents wifiev(logger);
mrwski::Server server(logger, wifimng, 80, nullptr);
mrwski::LEDStrip leds(LED_R, LED_G, LED_B);

int fs_init() {
    bool ok = LittleFS.begin();
    if (!ok) {
        logger.println("failed to setup FS");
        return 1;
    }

    logger.println("FS init ok");

    fs::FSInfo i;
    LittleFS.info(i);
    logger.printf("Used: %d / %d bytes (free %d bytes)\n", i.usedBytes, i.totalBytes, i.totalBytes - i.usedBytes);

    // logger.println("files present:");
    // listAllFilesInDir("/");
    // logger.println("");

    return 0;
}

#define NTP_SERVER "pl.pool.ntp.org"
#define TZ "CET-1CEST,M3.5.0,M10.5.0/3"

// TODO status led + integrate with wifi
// TODO route injection to Server
// TODO led control + simple frontend

void setup() {
    Serial.begin(115200);
    Serial.println();
    logger.begin_serial(Serial);

    fs_init();
    logger.begin_fs("/log");

    wifimng.begin();
    wifiev.register_events();

    server.begin();
    leds.begin();

    configTime(TZ, NTP_SERVER);
}

void loop() {
    wifimng.loop();
    server.loop();
}