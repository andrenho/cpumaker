#ifndef BOARDUI_HH
#define BOARDUI_HH

#include <SDL2/SDL.h>
#include "game/board.hh"

enum Icon: uint16_t;

class BoardUI {
public:
    void load_resources(SDL_Renderer* ren);
    bool update(SDL_Event* e);
    void draw(SDL_Renderer* ren) const;

    void set_board(Board* board) { this->board_ = board; }

private:
    Board* board_ = nullptr;
    SDL_Texture* icons_ = nullptr;
    int board_x = 30;
    int board_y = 30;

    void draw_board_borders(SDL_Renderer* ren) const;
    void draw_tile(SDL_Renderer* ren, int x, int y) const;
    void draw_icon(SDL_Renderer* ren, Icon icon, int x, int y) const;

    float zoom_ = 2.f;

    //
    // icon definition
    //

};

#endif //BOARDUI_HH
