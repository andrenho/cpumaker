#include "boardui.hh"

#include <SDL_image.h>

#include "battery/embed.hpp"

static constexpr int TILE_SIZE = 16;

struct Rect { uint8_t x; uint8_t y; uint8_t w=1; uint8_t h=1; };
enum Icon {
    I_TILE = 0,
};
static Rect rect[] = {
    { 2, 2 },
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

    for (size_t x = 0; x < board_->w(); ++x) {
        for (size_t y = 0; y < board_->h(); ++y) {
            draw_tile(ren, x, y);
        }
    }

    SDL_RenderSetScale(ren, 1.f, 1.f);
}

void BoardUI::draw_tile(SDL_Renderer* ren, int x, int y) const
{
    auto rect_to_sdlrect = [](Icon icon) {
        Rect const& r = rect[icon];
        return SDL_Rect { .x = r.x * TILE_SIZE, .y = r.y * TILE_SIZE, .w = r.w * TILE_SIZE, .h = r.h * TILE_SIZE };
    };

    // base
    SDL_Rect src = rect_to_sdlrect(I_TILE);
    SDL_Rect dest = { .x = board_x + (x * TILE_SIZE), .y = board_y + (y * TILE_SIZE), .w = src.w, .h = src.h };
    SDL_RenderCopy(ren, icons_, &src, &dest);
}
