#ifndef BOARDUI_HH
#define BOARDUI_HH

#include <optional>

#include <SDL2/SDL.h>

#include "boardsprites.hh"
#include "game/board.hh"

class BoardUI {
public:
    void load_resources(SDL_Renderer* ren);

    void event(SDL_Window* window, SDL_Event* e);
    void draw(SDL_Renderer* ren) const;

    void set_board(Board* board) { this->board_ = board; }

private:
    BoardSpriteSheet spritesheet_;
    Board*           board_ = nullptr;
    int              rel_x_ = 30;
    int              rel_y_ = 30;

    void draw_board_borders(SDL_Renderer* ren) const;
    void draw_tile(SDL_Renderer* ren, ssize_t x, ssize_t y) const;
    void draw_icon(SDL_Renderer* ren, BoardSpriteSheet::Sprite icon, ssize_t x, ssize_t y) const;
    void draw_temporary_wire(SDL_Renderer* ren, Position const& start, Position const& end, WireType wire_type) const;

    [[nodiscard]] std::optional<SubPosition> mouse_tile() const;

    float zoom_ = 2.f;
    bool  moving_ = false;
    std::optional<std::pair<Position, WireType>> drawing_wire_;
};

#endif //BOARDUI_HH
