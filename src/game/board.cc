#include "board.hh"

#include <algorithm>
#include <ranges>

namespace r = std::ranges;

void Board::paste(Board const& shadow, ssize_t x, ssize_t y, bool clear_empty_squares)
{
    // clear area
    if (clear_empty_squares) {
        ssize_t w = 0, h = 0;
        for (auto const& [pos, _]: shadow.components) {
            w = std::max(w, pos.x);
            h = std::max(h, pos.y);
        }
        for (ssize_t tx = x; tx < (x + w); ++tx)
            for (ssize_t ty = y; ty < (y + h); ++ty)
                components.erase({ tx, ty });
    }

    // paste components
    for (auto const& [pos, component]: shadow.components) {
        auto [tx, ty] = pos;
        if ((tx + x) >= 0 && (tx + x) < w_ && (ty + y) >= 0 && (ty + y) < h_) {
            components[{ (tx + x), (ty + y) }] = component;
        }
    }
}

std::vector<SubPosition> Board::route_wire(Position const& start, Position const& end)
{
    std::vector<SubPosition> sp;

    if (end.x > start.x) {
        sp.push_back({ .pos = start, .dir = Direction::E });
        for (ssize_t x = start.x + 1; x < end.x; ++x) {
            sp.push_back({ .pos = { x, start.y }, .dir = Direction::W });
            sp.push_back({ .pos = { x, start.y }, .dir = Direction::E });
        }
        sp.push_back({ .pos = { end.x, start.y }, .dir = Direction::W });
    } else if (end.x < start.x) {
        sp.push_back({ .pos = start, .dir = Direction::W });
        for (ssize_t x = start.x - 1; x > end.x; --x) {
            sp.push_back({ .pos = { x, start.y }, .dir = Direction::E });
            sp.push_back({ .pos = { x, start.y }, .dir = Direction::W });
        }
        sp.push_back({ .pos = { end.x, start.y }, .dir = Direction::E });
    }

    if (end.y > start.y) {
        sp.push_back({ .pos = { end.x, start.y }, .dir = Direction::S });
        for (ssize_t y = start.y + 1; y < end.y; ++y) {
            sp.push_back({ .pos = { end.x, y }, .dir = Direction::N });
            sp.push_back({ .pos = { end.x, y }, .dir = Direction::S });
        }
        sp.push_back({ .pos = end, .dir = Direction::N });
    } else if (end.y < start.y) {
        sp.push_back({ .pos = { end.x, start.y }, .dir = Direction::N });
        for (ssize_t y = start.y - 1; y > end.y; --y) {
            sp.push_back({ .pos = { end.x, y }, .dir = Direction::S });
            sp.push_back({ .pos = { end.x, y }, .dir = Direction::N });
        }
        sp.push_back({ .pos = end, .dir = Direction::S });
    }

    return sp;
}
