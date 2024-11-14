#ifndef COMPONENTS_HH
#define COMPONENTS_HH

#include <cstdint>

struct Component {
    enum Type { NPN, PNP, BUTTON, LED };

    Type    type;
    uint8_t value = 0;
};

#endif //COMPONENTS_HH
