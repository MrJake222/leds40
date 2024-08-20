#include <Arduino.h>

#include <hw.hpp>

#include <logger.hpp>
#include <ledstatus.hpp>
#include <ledstrip.hpp>
#include <wifimng.hpp>
#include <wifievents.hpp>
#include <server.hpp>

void config_changed();

mrwski::Logger logger;
mrwski::LEDStatus ledst(LED_STATUS);
mrwski::LEDStrip leds(LED_R, LED_G, LED_B);

wl_status_t sta_fail_reason;

mrwski::WifiEvents wifiev(logger);
mrwski::WifiMng wifimng(logger, ledst, config_changed, sta_fail_reason);
mrwski::Server server(logger, wifimng, 80, config_changed, sta_fail_reason);

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
// TODO FTP server

void setup() {
    Serial.begin(115200);
    Serial.println();
    logger.begin_serial(Serial);

    fs_init();
    logger.begin_fs("/log");

    leds.begin();
    ledst.begin();

    wifimng.begin();
    wifiev.register_events();

    server.begin();

    configTime(TZ, NTP_SERVER);
}

void config_changed() {
    server.restart();
}

void loop() {
    wifimng.loop();
    server.loop();
    ledst.loop();

    if (WiFi.status() == WL_CONNECTED) {
        // if web server reports no activity long periods of time
        // delay to enter light sleep mode, 75mA -> 35mA current draw reduction
        // and less heat from AMS1117
        if (server.ms_since_last_conn() > 5e3)
            delay(250); // 5 seconds -> 250ms delays
    }
}