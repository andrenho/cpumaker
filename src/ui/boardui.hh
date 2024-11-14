#ifndef BOARDUI_HH
#define BOARDUI_HH

#include <optional>

#include <SDL2/SDL.h>
#include "game/board.hh"

enum Icon: uint16_t;

class BoardUI {
public:
    void load_resources(SDL_Renderer* ren);
    void event(SDL_Window* window, SDL_Event* e);
    void draw(SDL_Renderer* ren) const;

    void set_board(Board* board) { this->board_ = board; }

private:
    Board* board_ = nullptr;
    SDL_Texture* icons_ = nullptr;
    int rel_x_ = 30;
    int rel_y_ = 30;

    void draw_board_borders(SDL_Renderer* ren) const;
    void draw_tile(SDL_Renderer* ren, ssize_t x, ssize_t y) const;
    void draw_icon(SDL_Renderer* ren, Icon icon, ssize_t x, ssize_t y) const;

    [[nodiscard]] std::optional<std::pair<ssize_t, ssize_t>> mouse_tile() const;

    float zoom_ = 2.f;
    bool  moving_ = false;
};

#endif //BOARDUI_HH
