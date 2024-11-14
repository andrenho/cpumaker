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
            w = std::max(w, pos.first);
            h = std::max(h, pos.second);
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
