#ifndef BOARDUI_HH
#define BOARDUI_HH

#include <SDL2/SDL.h>
#include "game/board.hh"

class BoardUI {
public:
    void load_resources(SDL_Renderer* ren);
    bool update(SDL_Event* e);
    void draw(SDL_Renderer* ren) const;

    void set_board(Board* board) { this->board_ = board; }

private:
    Board* board_ = nullptr;
    SDL_Texture* icons_ = nullptr;
    int board_x = 20;
    int board_y = 20;

    void draw_tile(SDL_Renderer* ren, int x, int y) const;

    float zoom_ = 3.f;
};

#endif //BOARDUI_HH
