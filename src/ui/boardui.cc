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

bool BoardUI::update(SDL_Event* e)
{
    return false;
}

void BoardUI::draw(SDL_Renderer* ren) const
{
    SDL_RenderSetScale(ren, zoom_, zoom_);

    draw_board_borders(ren);

    for (size_t x = 0; x < board_->w(); ++x) {
        for (size_t y = 0; y < board_->h(); ++y) {
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

    for (size_t x = 0; x < board_->w(); ++x) {
        draw_icon(ren, I_BOARD_T, x, -1);
        draw_icon(ren, I_BOARD_B, x, board_->h());
    }

    for (size_t y = 0; y < board_->h(); ++y) {
        draw_icon(ren, I_BOARD_L, -1, y);
        draw_icon(ren, I_BOARD_R, board_->w(), y);
    }
}

void BoardUI::draw_tile(SDL_Renderer* ren, int x, int y) const
{
    draw_icon(ren, I_TILE, x, y);
}

void BoardUI::draw_icon(SDL_Renderer* ren, Icon icon, int x, int y) const
{
    Rect const& r = rect[icon];
    SDL_Rect src { .x = r.x * TILE_SIZE, .y = r.y * TILE_SIZE, .w = r.w * TILE_SIZE, .h = r.h * TILE_SIZE };
    SDL_Rect dest = { .x = board_x + (x * TILE_SIZE), .y = board_y + (y * TILE_SIZE), .w = src.w, .h = src.h };
    SDL_RenderCopy(ren, icons_, &src, &dest);
}
