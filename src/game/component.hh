#ifndef COMPONENTS_HH
#define COMPONENTS_HH

#include <cstdint>
#include <variant>

#include "position.hh"

//
// COMPONENTS
//

struct Element {
    enum Type { NPN, PNP, BUTTON, LED };

    Type    type;
    uint8_t value = 0;
};

using Component = std::variant<Element>;

//
// WIRES
//

enum class WireWidth : uint8_t { W1 };
enum class WireSide : uint8_t { Top, Bottom };

struct WireConfiguration {
    WireWidth width;
    WireSide  side;
    Direction dir;

    bool operator==(WireConfiguration const& o) const { return width == o.width && side == o.side && dir == o.dir; }
};

#endif //COMPONENTS_HH
