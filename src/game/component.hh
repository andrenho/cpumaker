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
    bool      value = false;

    bool operator==(WireConfiguration const& o) const { return width == o.width && side == o.side && dir == o.dir && value == o.value; }
};

template <>
struct std::hash<WireConfiguration> {
    std::size_t operator()(WireConfiguration const& wc) const noexcept {
        return ((((uint8_t) wc.dir) << 16) | (((uint8_t) wc.side) << 8) | (uint8_t) wc.width) << wc.value;
    }
};

#endif //COMPONENTS_HH
