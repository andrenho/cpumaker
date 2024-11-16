#ifndef BOARD_HH
#define BOARD_HH

#include <unordered_map>
#include <variant>
#include <vector>
#include <sys/types.h>

#include "component.hh"
#include "position.hh"

class Board {
public:
    Board() : w_(10), h_(10) {}

    [[nodiscard]] ssize_t w() const { return w_; }
    [[nodiscard]] ssize_t h() const { return h_; }

    std::unordered_map<Position, Component> components {};

    void paste(Board const& shadow, ssize_t x, ssize_t y, bool clear_empty_squares);

    static std::vector<SubPosition> wire_from_a_to_b(Position const& start, Position const& end);

private:
    ssize_t w_, h_;
};

#endif //BOARD_HH
