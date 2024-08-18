#pragma once

#include <logger.hpp>
#include <ESP8266WiFi.h>

namespace mrwski {

class WifiEvents {

    Logger& logger;

    void mac_to_string(char* buf, const unsigned char* mac);
    void print_disconnect_reason(WiFiDisconnectReason reason);

    WiFiEventHandler handler_StationModeConnected;
    WiFiEventHandler handler_StationModeDisconnected;
    WiFiEventHandler handler_StationModeAuthModeChanged;
    WiFiEventHandler handler_StationModeGotIP;
    WiFiEventHandler handler_StationModeDHCPTimeout;
    WiFiEventHandler handler_SoftAPModeStationConnected;
    WiFiEventHandler handler_SoftAPModeStationDisconnected;
    WiFiEventHandler handler_SoftAPModeProbeRequestReceived;
    WiFiEventHandler handler_WiFiModeChange;

    void wifi_event_StationModeConnected(const WiFiEventStationModeConnected& data);
    void wifi_event_StationModeDisconnected(const WiFiEventStationModeDisconnected& data);
    void wifi_event_StationModeAuthModeChanged(const WiFiEventStationModeAuthModeChanged& data);
    void wifi_event_StationModeGotIP(const WiFiEventStationModeGotIP& data);
    void wifi_event_StationModeDHCPTimeout();
    void wifi_event_SoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected& data);
    void wifi_event_SoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& data);
    void wifi_event_SoftAPModeProbeRequestReceived(const WiFiEventSoftAPModeProbeRequestReceived& data);
    void wifi_event_WiFiModeChange(const WiFiEventModeChange& data);

public:
    WifiEvents(Logger& logger_)
        : logger(logger_)
        { }

    void register_events();
};

} // end namespace