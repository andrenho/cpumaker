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
    void draw_icon(SDL_Renderer* ren, BoardSpriteSheet::Sprite icon, ssize_t x, ssize_t y, bool semitransparent=false) const;

    [[nodiscard]] std::optional<SubPosition> mouse_tile() const;

    float zoom_ = 2.f;
    bool  dragging_board_ = false;

    // draw wire

    struct TempWire {
        Position                   start_pos;
        WireWidth                  width;
        WireSide                   side;
        std::optional<Orientation> orientation {};
    };
    std::optional<TempWire> drawing_wire_;
    void draw_temporary_wire(SDL_Renderer* ren, TempWire const& temp_wire, Position const& end) const;
    void process_move_while_drawing_wire();
};

#endif //BOARDUI_HH
