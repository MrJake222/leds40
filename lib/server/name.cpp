#include "name.hpp"

#include <LittleFS.h>

namespace mrwski {

int Name::set(const char* name) {
    uint len = strlen(name);

    File file = LittleFS.open("/name", "w");
    file.write((const uint8_t*)&len, sizeof(len));
    file.write(name);
    file.close();

    return 0;
}

int Name::get(char* name, uint maxlen) {
    uint len;

    File file = LittleFS.open("/name", "r");
    if (!file) {
        name[0] = '\0';
        return 1;
    }

    file.read((uint8_t*)&len, sizeof(len));
    file.read((uint8_t*)name, std::min(len, maxlen));
    file.close();

    return 0;
}

} // end namespace