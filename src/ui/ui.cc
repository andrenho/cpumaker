#include "ui.hh"

#include <string>
using namespace std::string_literals;

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

#include "battery/embed.hpp"

UI::UI(Game& game)
    : game_(game)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    window_ = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 800, SDL_WINDOW_RESIZABLE);
    if (!window_)
        throw std::runtime_error("Error: SDL_CreateWindow(): "s + SDL_GetError());

    ren_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!ren_)
        throw std::runtime_error("Error: SDL_CreateRenderer(): "s + SDL_GetError());

    SDL_RendererInfo info;
    SDL_GetRendererInfo(ren_, &info);
    SDL_Log("Current SDL_Renderer: %s", info.name);

    load_resources();
    board_ui_.load_resources(ren_);

    board_ui_.set_board(&game.board());
}

void UI::load_resources()
{
    auto bg = b::embed<"resources/images/bg.jpg">();
    SDL_Surface* sf = IMG_Load_RW(SDL_RWFromMem((void *) bg.data(), (int) bg.size()), 1);
    bg_texture_ = SDL_CreateTextureFromSurface(ren_, sf);
    SDL_FreeSurface(sf);

    auto font_file = b::embed<"resources/fonts/04B_03__.TTF">();
    font_ = TTF_OpenFontRW(SDL_RWFromMem((void *) font_file.data(), (int) font_file.size()), 1, 16);
}


UI::~UI()
{
    if (font_) TTF_CloseFont(font_);
    if (img_texture_) SDL_DestroyTexture(img_texture_);
    if (bg_texture_) SDL_DestroyTexture(bg_texture_);
    if (ren_) SDL_DestroyRenderer(ren_);
    if (window_) SDL_DestroyWindow(window_);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void UI::update(Duration timestep)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {

        switch (e.type) {
            case SDL_QUIT:
                running_ = false;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_q)
                    running_ = false;
                break;
            default: break;
        }

        board_ui_.update(&e);  // TODO - check where the event is happening
    }
}

void UI::render()
{
    // clear screen
    SDL_RenderCopy(ren_, bg_texture_, nullptr, nullptr);

    board_ui_.draw(ren_);

    SDL_RenderPresent(ren_);
}

