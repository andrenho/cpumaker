#ifndef GAME_HH
#define GAME_HH
#include "board.hh"

class Game {
public:
    [[nodiscard]] Board const& board() const { return board_; }
    [[nodiscard]] Board& board() { return board_; }

private:
    Board board_;
};

#endif //GAME_HH
