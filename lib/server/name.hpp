#pragma once

#include <sys/types.h>

namespace mrwski {

class Name {

public:
    int set(const char* name);
    int get(char* name, uint buflen);
};

} // end namespace