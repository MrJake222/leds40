#pragma once

#include <ESP8266WebServer.h>

namespace mrwski {

class EspServer : public ESP8266WebServer {

public:
    EspServer(short port) : ESP8266WebServer(port) { }
    unsigned long get_statusChange() { return _statusChange; }
};

} // end namespace