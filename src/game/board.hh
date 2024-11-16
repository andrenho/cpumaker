#ifndef BOARD_HH
#define BOARD_HH

#include <unordered_map>
#include <unordered_set>
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

    void add_component(Position const& pos, Component const& component) { components_[pos] = component; }
    void add_wire(Position const& pos, WireConfiguration const& wire) { wires_[pos].emplace(wire); }
    void paste(Board const& shadow, ssize_t x, ssize_t y, bool clear_empty_squares);

    [[nodiscard]] std::unordered_map<Position, Component> const&                             components() const { return components_; }
    [[nodiscard]] std::unordered_map<Position, std::unordered_set<WireConfiguration>> const& wires() const { return wires_; }

    static std::vector<SubPosition> route_wire(Position const& start, Position const& end);

private:
    ssize_t w_, h_;
    std::unordered_map<Position, Component> components_ {};
    std::unordered_map<Position, std::unordered_set<WireConfiguration>> wires_ {};
};

#endif //BOARD_HH
