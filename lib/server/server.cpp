#include "server.hpp"

#include <LittleFS.h>

namespace mrwski {

void Server::begin() {
    esp_server.begin();

    esp_server.on("/wifi_scan", HTTP_GET, [this] { handle_wifi_scan(); });
    esp_server.on("/wifi_status", HTTP_GET, [this] { handle_wifi_status(); });
    esp_server.on("/wifi_ap_config", HTTP_POST,[this] { handle_wifi_ap_config(); });
    esp_server.on("/wifi_sta_config", HTTP_POST, [this] { handle_wifi_sta_config(); });
    esp_server.on("/log", HTTP_GET, [this] { handle_log(); });
    esp_server.on("/name", HTTP_GET, [this] { handle_name_get(); });
    esp_server.on("/name", HTTP_POST, [this] { handle_name_post(); });

    esp_server.serveStatic("/", LittleFS, "/static/");

    logger.println("Server listening");
}

void Server::stop() {
    logger.println("Server stopping");
    esp_server.stop();
}

void Server::loop() {
    esp_server.handleClient();
}

void Server::serialize_and_send_json() {
    int contentLength = serializeJson(json_doc, json_buf, json_buf_size);
    esp_server.send(200, "application/json", json_buf, contentLength);
}

void Server::redirect(const String& url) {
    esp_server.sendHeader("Location", url);
    esp_server.send(301, "text/plain", "Redirection in progress...");
}

ulong Server::ms_since_last_conn() {
    return millis() - esp_server.get_statusChange();
}

void Server::handle_wifi_scan() {

    json_doc.clear();

    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();

    for (int i=0; i<n; i++) {
        json_doc[i]["ssid"] = WiFi.SSID(i);
        json_doc[i]["rssi"] = WiFi.RSSI(i);
        json_doc[i]["bssid"] = WiFi.BSSIDstr(i);
        json_doc[i]["channel"] = WiFi.channel(i);
        json_doc[i]["hidden"] = WiFi.isHidden(i);

        uint8_t enc = WiFi.encryptionType(i);
        switch (enc) {
            case ENC_TYPE_NONE: // open
                json_doc[i]["enc"] = "open";
                break;

            case ENC_TYPE_WEP: // wep
                json_doc[i]["enc"] = "wep";
                break;

            case ENC_TYPE_TKIP: // wpa
                json_doc[i]["enc"] = "wpa";
                break;

            case ENC_TYPE_CCMP: // wpa2
            case ENC_TYPE_AUTO: // wpa2
                json_doc[i]["enc"] = "wpa2";
                break;
            
            default:
                json_doc[i]["enc"] = "unknown";
        }

        json_doc[i]["connected"] = WiFi.isConnected() && (WiFi.SSID() == WiFi.SSID(i));
    }

    serialize_and_send_json();
}

void Server::handle_wifi_status() {

    struct WifiMng::wifi_mode mode = WifiMng::mode_to_struct(WiFi.getMode());

    json_doc.clear();
    json_doc["mode"] = mode.name;

    if (mode.ap) {
        json_doc["ap_ssid"] = WiFi.softAPSSID();
        json_doc["ap_ip"] = WiFi.softAPIP().toString();
    }

    if (mode.sta) {
        json_doc["sta_status_string"] = WifiMng::status_to_string(sta_fail_reason);
        json_doc["sta_ip"] = WiFi.localIP().toString();
    }

    serialize_and_send_json();
}

void Server::handle_wifi_ap_config() {
    String ap = esp_server.arg("ap");

    if (ap == "off") {
        logger.println("turning off AP mode");
        redirect("/config/status/ap_off.html");

        WiFi.softAPdisconnect(true);
    }

    else {
        logger.println("turning on AP mode");
        redirect("/config/status/ap_on.html");

        WiFi.persistent(true);
        WiFi.softAP("ESP8266");
    }

    if (config_changed_callback)
        config_changed_callback();
}

void Server::handle_wifi_sta_config() {
    String sta = esp_server.arg("sta");

    if (sta == "off") {
        logger.println("turning off STA mode");
        redirect("/config/status/sta_off.html");
        WiFi.disconnect(true);
    }

    else {
        if (!esp_server.hasArg("ssid")) {
            redirect("/config/status/sta_e_ssid.html");
            return;
        }

        String ssid = esp_server.arg("ssid");
        String pass = esp_server.arg("pass");

        logger.println("turning on STA mode");
        redirect("/config/status/sta_on.html");

        logger.println("Trying:");
        logger.println("  SSID: " + ssid);
        logger.println("  pass: " + pass);

        WiFi.setAutoReconnect(true);
        WiFi.persistent(true);
        WiFi.begin(ssid, pass);
        
        logger.println("waiting for STA connection");

        // We don't call config_changed() or anything
        // because we need to wait till ESP connects
        wifimng.wait_for_sta(true);
    }
}

void Server::handle_log() {
    logger.flush();

    File file = LittleFS.open("/log", "r");
    esp_server.streamFile(file, "text/plain");
    file.close();
}

void Server::handle_name_get() {
    json_doc.clear();

    char namestr[32];
    name.get(namestr, 32);
    json_doc["name"] = String(namestr);

    serialize_and_send_json();
}

void Server::handle_name_post() {
    if (!esp_server.hasArg("name")) {
        redirect("/config/status/name_err.html");
        return;
    }

    name.set(esp_server.arg("name").c_str());
    redirect("/config/status/name_ok.html");
}


} // end namespace