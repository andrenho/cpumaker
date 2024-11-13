#include "boardui.hh"

#include <SDL_image.h>

#include "battery/embed.hpp"

static constexpr int TILE_SIZE = 16;

struct Rect { uint8_t x; uint8_t y; uint8_t w=1; uint8_t h=1; };
enum Icon : uint16_t {
    I_TILE = 0,
    I_BOARD_TL, I_BOARD_T, I_BOARD_TR, I_BOARD_L, I_BOARD_R, I_BOARD_BL, I_BOARD_B, I_BOARD_BR,
};
static constexpr Rect rect[] = {
    { 2, 2 },       // I_TILE
    { 0, 0, 2, 2 }, // I_BOARD_*
    { 2, 0, 1, 2 },
    { 3, 0, 2, 2 },
    { 0, 2, 2, 1 },
    { 3, 2, 2, 1 },
    { 0, 3, 2, 2 },
    { 2, 3, 1, 2 },
    { 3, 3, 2, 2 },
};

void BoardUI::load_resources(SDL_Renderer* ren)
{
    auto bg = b::embed<"resources/images/circuit.png">();
    SDL_Surface* sf = IMG_Load_RW(SDL_RWFromMem((void *) bg.data(), (int) bg.size()), 1);
    // TODO - set color key
    icons_ = SDL_CreateTextureFromSurface(ren, sf);
    SDL_FreeSurface(sf);
}

void BoardUI::event(SDL_Window* window, SDL_Event* e)
{
    switch (e->type) {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            if (e->button.button == SDL_BUTTON_RIGHT) {
                moving_ = (e->button.state == SDL_PRESSED);
                SDL_ShowCursor(moving_ ? SDL_DISABLE : SDL_ENABLE);
            }
            break;

        case SDL_MOUSEMOTION:
            if (moving_) {
                int scr_w, scr_h;
                SDL_GetWindowSize(window, &scr_w, &scr_h);
                rel_x_ += (e->motion.xrel / zoom_);
                rel_y_ += (e->motion.yrel / zoom_);
                rel_x_ = std::max(std::min(rel_x_, (int) (scr_w / zoom_ - 20)), (int) -(board_->w() * TILE_SIZE) + 20);
                rel_y_ = std::max(std::min(rel_y_, (int) (scr_h / zoom_ - 20)), (int) -(board_->h() * TILE_SIZE) + 20);
            }
            break;
    }
}

void BoardUI::draw(SDL_Renderer* ren) const
{
    SDL_RenderSetScale(ren, zoom_, zoom_);

    draw_board_borders(ren);

    for (ssize_t x = 0; x < board_->w(); ++x) {
        for (ssize_t y = 0; y < board_->h(); ++y) {
            draw_tile(ren, x, y);
        }
    }

    SDL_RenderSetScale(ren, 1.f, 1.f);
}

void BoardUI::draw_board_borders(SDL_Renderer* ren) const
{
    draw_icon(ren, I_BOARD_TL, -2, -2);
    draw_icon(ren, I_BOARD_TR, board_->w(), -2);
    draw_icon(ren, I_BOARD_BL, -2, board_->h());
    draw_icon(ren, I_BOARD_BR, board_->w(), board_->h());

    for (ssize_t x = 0; x < board_->w(); ++x) {
        draw_icon(ren, I_BOARD_T, x, -2);
        draw_icon(ren, I_BOARD_B, x, board_->h());
    }

    for (ssize_t y = 0; y < board_->h(); ++y) {
        draw_icon(ren, I_BOARD_L, -2, y);
        draw_icon(ren, I_BOARD_R, board_->w(), y);
    }
}

void BoardUI::draw_tile(SDL_Renderer* ren, ssize_t x, ssize_t y) const
{
    draw_icon(ren, I_TILE, x, y);
}

void BoardUI::draw_icon(SDL_Renderer* ren, Icon icon, ssize_t x, ssize_t y) const
{
    Rect const& r = rect[icon];
    SDL_Rect src { .x = r.x * TILE_SIZE, .y = r.y * TILE_SIZE, .w = r.w * TILE_SIZE, .h = r.h * TILE_SIZE };
    SDL_Rect dest = { .x = (int) (rel_x_ + (x * TILE_SIZE)), .y = (int) (rel_y_ + (y * TILE_SIZE)), .w = src.w, .h = src.h };
    SDL_RenderCopy(ren, icons_, &src, &dest);
}
