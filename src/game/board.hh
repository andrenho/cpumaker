#ifndef BOARD_HH
#define BOARD_HH

class Board {
public:
    Board() : w_(10), h_(10) {}

    [[nodiscard]] ssize_t w() const { return w_; }
    [[nodiscard]] ssize_t h() const { return h_; }

private:
    ssize_t w_, h_;
};

#endif //BOARD_HH
