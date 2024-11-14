#ifndef BOARD_HH
#define BOARD_HH

#include <map>
#include <variant>
#include <sys/types.h>

#include "component.hh"

using Layout = std::map<std::pair<ssize_t, ssize_t>, Component>;

class Board {
public:
    Board() : w_(10), h_(10) {}

    [[nodiscard]] ssize_t w() const { return w_; }
    [[nodiscard]] ssize_t h() const { return h_; }

    Layout components {};

    void paste(Board const& shadow, ssize_t x, ssize_t y, bool clear_empty_squares);

private:
    ssize_t w_, h_;
};

#endif //BOARD_HH
