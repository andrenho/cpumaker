#ifndef RESOURCES_HH
#define RESOURCES_HH

#include <vector>
#include <SDL2/SDL.h>

#include "game/component.hh"

class BoardSpriteSheet {
public:
    struct Coordinate {
        uint8_t x;
        uint8_t y;
        uint8_t w = 1;
        uint8_t h = 1;
    };

    enum class Sprite;

    struct BoardSprite {
        Sprite top_left, top_right, bottom_left, bottom_right,
               top, bottom, left, right, inner;
    };

    void load(SDL_Renderer* ren);

    [[nodiscard]] SDL_Texture* texture() const { return texture_; }

    [[nodiscard]] static Coordinate const&   coordinate(Sprite sprite);
    [[nodiscard]] static BoardSprite const&  board_sprites();
    [[nodiscard]] static std::vector<Sprite> component_sprites(Component const& component);
    [[nodiscard]] static Sprite              wire_sprite(WireConfiguration const& wire, bool value);

private:
    SDL_Texture* texture_ = nullptr;
};

#endif //RESOURCES_HH
