#include "wifimng.hpp"

#include <ESP8266WiFi.h>

namespace mrwski {

void WifiMng::begin() {
    logger.println("setting up AP on boot");
    WiFi.softAP("ESP8266");
    logger.print("AP IP address: ");
    logger.println(WiFi.softAPIP());

    last_check_ms = 0;
    ap_time = 0;
    ap = true;
    sta_check = false;
}

void WifiMng::ap_loop() {
    if (ap_time < ap_time_max) {
        // wait in AP mode for connection
        // (to possibly apply initial configuration for the module)

        if (wifi_softap_get_station_num() > 0) {
            // if anything is connected, wait
            ap_time = 0;
        }

        else {
            if (ap_time % 10 == 0) {
                logger.printf("Waiting for AP connection: %d/%d\n", ap_time, ap_time_max);
            }

            ap_time++;
            logger.print(".");
        }
    }

    else {
        // time's up, enable STA mode
        ap = false;
        WiFi.softAPdisconnect(true);

        WiFi.begin();
        logger.println("Enabling STA mode with saved config");

        wait_for_sta();
    }
}

void WifiMng::sta_loop() {
    if (WiFi.status() != WL_CONNECTED) {
        if (sta_tries % 10 == 0) {
            logger.printf("Connecting to %s, try %d, status: %s\n",
                          WiFi.SSID().c_str(),
                          sta_tries,
                          sta_status_to_string(WiFi.status()).c_str());
        }

        if (WiFi.status() != WL_DISCONNECTED) {
            // Don't give up
            WiFi.begin();
        }

        sta_tries++;
        logger.print(".");
    }

    else {
        // Success!
        // status CONNECTED

        // config_changed();
        // server.restart();

        logger.println("Connected in STA mode");
        logger.println("IP: " + WiFi.localIP().toString());

        sta_check = false;
    }
}

void WifiMng::loop() {
    if ((millis() - last_check_ms) >= 1000) {
        if (ap)         ap_loop();
        if (sta_check)  sta_loop();

        last_check_ms = millis();
    }
}

void WifiMng::wait_for_sta() {
    sta_check = true;
    sta_tries = 0;
}

struct WifiMng::wifi_mode WifiMng::mode_to_struct(int code) {

    struct wifi_mode mode;
    mode.ap = false;
    mode.sta = false;

    // WIFI_OFF = 0, WIFI_STA = 1, WIFI_AP = 2, WIFI_AP_STA = 3
    switch (code) {
        case WIFI_OFF:
            mode.name = "off";
            break;

        case WIFI_STA:
            mode.name = "sta";
            mode.sta = true;
            break;

        case WIFI_AP:
            mode.name = "ap";
            mode.ap = true;
            break;

        case WIFI_AP_STA:
            mode.name = "ap_sta";
            mode.ap = true;
            mode.sta = true;
            break;

        default:
            // TODO return code
            mode.name = "unknown";
    }

    return mode;
}

String WifiMng::sta_status_to_string(int status) {
    switch (status) {
        case WL_CONNECTED:
            return "Connected to " + WiFi.SSID();

        case WL_NO_SSID_AVAIL:
            return "No such network";

        case WL_CONNECT_FAILED:
            return "Connection failed";

        case WL_WRONG_PASSWORD:
            return "Wrong password";

        case WL_IDLE_STATUS:
            return "Idle";

        case WL_DISCONNECTED:
            return "Disconnected";

        default:
            // TODO return code
            return "Unknown error";
    }
}

} // end namespace
