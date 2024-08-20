#include "wifimng.hpp"

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
            ledst.ap_conn();
        }

        else {
            if (ap_time % 10 == 0) {
                logger.printf("Waiting for AP connection: %d/%d\n", ap_time, ap_time_max);
            }

            ap_time++;
            logger.dot();
            ledst.ap_wait();
        }
    }

    else {
        // time's up, enable STA mode
        ap = false;
        WiFi.softAPdisconnect(true);

        WiFi.begin();
        logger.println("Enabling STA mode with saved config");

        wait_for_sta(false);
    }
}

void WifiMng::sta_loop() {
    wl_status_t status = WiFi.status();
    if (sta_time % 10 == 0) {
        logger.printf("Connecting to %s, try %d, status: %s\n",
                      WiFi.SSID().c_str(),
                      sta_time,
                      status_to_string(status));
    }

    if (sta_fail && (sta_time >= sta_fail_time)) {
        logger.printf("Giving up after %d/%d tries, error: %s\n", sta_time, sta_fail_time, status_to_string(sta_fail_reason));
        WiFi.disconnect();
        sta_check = false;
        ledst.sta_err();
    }

    else if (status == WL_CONNECTED) {
        // Success!
        logger.printf("Connected to %s in %d tries\n", WiFi.SSID().c_str(), sta_time);
        sta_fail_reason = status;
        sta_check = false;
        ledst.sta_ok();
        if (config_changed_callback)
            config_changed_callback();

        // in case the connection from AP succeeded
        // don't switch anymore, the AP should be disabled manually by the user
        ap = false;
    }

    else if (status == WL_DISCONNECTED) {
        // no operation, connection in progress
    }

    else if (status == WL_IDLE_STATUS) {
        // no wifi configured, or aborted
        sta_check = false;
        ledst.sta_err();
    }

    else {
        // failed
        logger.printf("Failed to connect, try %d, reason %s\n", sta_time, status_to_string(status));
        sta_fail_reason = status;
        // don't give up
        WiFi.begin();
    }

    logger.dot();
    sta_time++;
}

void WifiMng::loop() {
    if ((millis() - last_check_ms) >= (1000 / tps)) {
        if (sta_check)  sta_loop();
        else if (ap)    ap_loop();

        last_check_ms = millis();
    }
}

void WifiMng::wait_for_sta(bool fail) {
    sta_check = true;
    sta_time = 0;
    sta_fail = fail;
    ledst.sta_wait();
}

struct WifiMng::wifi_mode WifiMng::mode_to_struct(WiFiMode_t code) {

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

const char* WifiMng::status_to_string(wl_status_t status) {
    switch (status) {
        case WL_CONNECTED:
            return "Connected";

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
