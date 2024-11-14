#ifndef BOARD_HH
#define BOARD_HH

#include <map>
#include <variant>

#include "component.hh"

class Board {
public:
    Board() : w_(10), h_(10) {}

    [[nodiscard]] ssize_t w() const { return w_; }
    [[nodiscard]] ssize_t h() const { return h_; }

    std::map<std::pair<ssize_t, ssize_t>, std::variant<Component>> components {};

private:
    ssize_t w_, h_;
};

#endif //BOARD_HH
