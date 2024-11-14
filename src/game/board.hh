#ifndef BOARD_HH
#define BOARD_HH

#include <unordered_map>
#include <variant>
#include <sys/types.h>

#include "component.hh"
#include "position.hh"

enum class WireType { None, W1 };
struct Wire {
    WireType upper = WireType::None;
    WireType lower = WireType::None;
};

class Board {
public:
    Board() : w_(10), h_(10) {}

    [[nodiscard]] ssize_t w() const { return w_; }
    [[nodiscard]] ssize_t h() const { return h_; }

    std::unordered_map<Position, Component> components {};
    std::unordered_map<SubPosition, Wire>   wires {};

    void paste(Board const& shadow, ssize_t x, ssize_t y, bool clear_empty_squares);

private:
    ssize_t w_, h_;
};

#endif //BOARD_HH
