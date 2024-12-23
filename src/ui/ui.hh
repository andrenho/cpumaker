#ifndef UI_UI_HH_
#define UI_UI_HH_

#include <chrono>

#include "boardui.hh"
#include "game/game.hh"

using hr = std::chrono::high_resolution_clock;
using Duration = decltype(hr::now() - hr::now());

class UI {
public:
    explicit UI(Game& game);
    ~UI();
    UI (const UI&) = delete;
    UI& operator=(const UI&) = delete;

    void update([[maybe_unused]] Duration timestep);
    void render();

    [[nodiscard]] bool running() const { return running_; }

private:
    void load_resources();

    bool running_ = true;
    bool show_demo_window_ = true;

    Game&                game_;
    BoardUI              board_ui_;

    struct SDL_Window*   window_;
    struct SDL_Renderer* ren_;
    struct SDL_Texture*  bg_texture_ = nullptr;
    struct SDL_Texture*  img_texture_ = nullptr;
    struct _TTF_Font*    font_ = nullptr;
};

#endif
