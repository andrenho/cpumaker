#include "board.hh"

#include <algorithm>
#include <ranges>

namespace r = std::ranges;

void Board::paste(Board const& shadow, ssize_t x, ssize_t y, bool clear_empty_squares)
{
    // clear area
    if (clear_empty_squares) {
        ssize_t w = 0, h = 0;
        for (auto const& [pos, _]: shadow.components_) {
            w = std::max(w, pos.x);
            h = std::max(h, pos.y);
        }
        for (ssize_t tx = x; tx < (x + w); ++tx)
            for (ssize_t ty = y; ty < (y + h); ++ty)
                components_.erase({ tx, ty });
    }

    // paste components
    for (auto const& [pos, component]: shadow.components_) {
        auto [tx, ty] = pos;
        if ((tx + x) >= 0 && (tx + x) < w_ && (ty + y) >= 0 && (ty + y) < h_) {
            components_[{ (tx + x), (ty + y) }] = component;
        }
    }
}

std::vector<SubPosition> Board::route_wire(Position const& start, Position const& end, Orientation orientation)
{
    std::vector<SubPosition> sp;

    auto add_straight_wire = [&sp](Position const& from, Position const& to) {
        if (from.x == to.x && from.y < to.y) {   // vertical
            sp.push_back({ .pos = from, .dir = Direction::S });
            for (ssize_t y = from.y + 1; y < to.y; ++y) {
                sp.push_back({ .pos = { to.x, y }, .dir = Direction::N });
                sp.push_back({ .pos = { to.x, y }, .dir = Direction::S });
            }
            sp.push_back({ .pos = to, .dir = Direction::N });
        } else if (from.y == to.y && from.x < to.x) {  // horizontal
            sp.push_back({ .pos = from, .dir = Direction::E });
            for (ssize_t x = from.x + 1; x < to.x; ++x) {
                sp.push_back({ .pos = { x, from.y }, .dir = Direction::W });
                sp.push_back({ .pos = { x, from.y }, .dir = Direction::E });
            }
            sp.push_back({ .pos = to, .dir = Direction::W });
        } else {
            throw std::runtime_error("Invalid straight line.");
        }
    };

    if (orientation == Orientation::Horizontal) {
        if (start.x != end.x)
            add_straight_wire({ std::min(start.x, end.x), start.y }, { std::max(start.x, end.x), start.y });
        if (start.y != end.y)
            add_straight_wire({ end.x, std::min(start.y, end.y) }, {  end.x, std::max(start.y, end.y) });
    } else if (orientation == Orientation::Vertical) {
        if (start.y != end.y)
            add_straight_wire({ start.x, std::min(start.y, end.y) }, {  start.x, std::max(start.y, end.y) });
        if (start.x != end.x)
            add_straight_wire({ std::min(start.x, end.x), end.y }, { std::max(start.x, end.x), end.y });
    }

    return sp;
}
