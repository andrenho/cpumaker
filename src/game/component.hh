#ifndef COMPONENTS_HH
#define COMPONENTS_HH

#include <cstdint>
#include <variant>

struct Element {
    enum Type { NPN, PNP, BUTTON, LED };

    Type    type;
    uint8_t value = 0;
};

using Component = std::variant<Element>;

#endif //COMPONENTS_HH
