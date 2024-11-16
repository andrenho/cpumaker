#include "boardsprites.hh"

#include <unordered_map>
#include <SDL_image.h>

#include "battery/embed.hpp"

enum class BoardSpriteSheet::Sprite {
    Tile = 0,
    BoardTopLeft, BoardTop, BoardTopRight, BoardLeft, BoardRight, BoardBottomLeft, BoardBottom, BoardBottomRight,
    NPN, PNP, ButtonOff, ButtonOn, LedOff, LedOn,
    ShadowRect, ShadowSquare, ShadowCircle,
    WireTopOnNorth_1, WireTopOnEast_1, WireTopOnWest_1, WireTopOnSouth_1,
    WireTopOffNorth_1, WireTopOffEast_1, WireTopOffWest_1, WireTopOffSouth_1,
};

static constexpr BoardSpriteSheet::Coordinate coordinates[] = {
    { 2, 2 },       // Tile
    { 0, 0, 2, 2 }, // Board*
    { 2, 0, 1, 2 },
    { 3, 0, 2, 2 },
    { 0, 2, 2, 1 },
    { 3, 2, 2, 1 },
    { 0, 3, 2, 2 },
    { 2, 3, 1, 2 },
    { 3, 3, 2, 2 },
    { 5, 2 },       // NPN
    { 5, 3 },
    { 6, 2 },
    { 6, 3 },
    { 7, 2 },
    { 7, 3 },
    { 5, 4 },       // Shadow*
    { 6, 4 },
    { 7, 4 },
    { 0, 7 }, { 1, 7 }, { 2, 7 }, { 3, 7 },  // WireTopOn*
    { 0, 5 }, { 1, 5 }, { 2, 5 }, { 3, 5 },  // WireTopOff*
};

void BoardSpriteSheet::load(SDL_Renderer* ren)
{
    auto bg = b::embed<"resources/images/circuit.png">();
    SDL_Surface* sf = IMG_Load_RW(SDL_RWFromMem((void *) bg.data(), (int) bg.size()), 1);
    texture_ = SDL_CreateTextureFromSurface(ren, sf);
    SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(sf);
}

BoardSpriteSheet::Coordinate const& BoardSpriteSheet::coordinate(Sprite sprites)
{
    return coordinates[(size_t) sprites];
}

BoardSpriteSheet::BoardSprite const& BoardSpriteSheet::board_sprites()
{
    using enum Sprite;
    constexpr static BoardSprite board_tile = {
        .top_left     = BoardTopLeft,
        .top_right    = BoardTopRight,
        .bottom_left  = BoardBottomLeft,
        .bottom_right = BoardBottomRight,
        .top          = BoardTop,
        .bottom       = BoardBottom,
        .left         = BoardLeft,
        .right        = BoardRight,
        .inner        = Tile,
    };
    return board_tile;
}

std::vector<BoardSpriteSheet::Sprite> BoardSpriteSheet::component_sprites(Component const& component)
{
    std::vector<Sprite> sprites;

    if (auto* element = std::get_if<Element>(&component); element) {
        switch (element->type) {
            case Element::NPN:
                sprites.push_back(Sprite::ShadowRect);
                sprites.push_back(Sprite::NPN);
                break;
            case Element::PNP:
                sprites.push_back(Sprite::ShadowRect);
                sprites.push_back(Sprite::PNP);
                break;
            case Element::BUTTON:
                sprites.push_back(Sprite::ShadowSquare);
                sprites.push_back(element->value ? Sprite::ButtonOn : Sprite::ButtonOff);
                break;
            case Element::LED:
                sprites.push_back(Sprite::ShadowCircle);
                sprites.push_back(element->value ? Sprite::LedOn : Sprite::LedOff);
                break;
        }
    }

    return sprites;
}

// hash for wire sprite list
template <>
struct std::hash<std::pair<WireConfiguration, bool>> {
    std::size_t operator()(std::pair<WireConfiguration, bool> const& pair) const noexcept {
        auto const [wc, value] = pair;
        return ((((uint8_t) wc.dir) << 16) | (((uint8_t) wc.side) << 8) | (uint8_t) wc.width) << value;
    }
};

BoardSpriteSheet::Sprite BoardSpriteSheet::wire_sprite(WireConfiguration const& wire, bool value)
{
    static const std::unordered_map<std::pair<WireConfiguration, bool>, Sprite> configs {
        { { { WireWidth::W1, WireSide::Top, Direction::N }, true }, Sprite::WireTopOnNorth_1 },
        { { { WireWidth::W1, WireSide::Top, Direction::S }, true }, Sprite::WireTopOnSouth_1 },
        { { { WireWidth::W1, WireSide::Top, Direction::W }, true }, Sprite::WireTopOnWest_1 },
        { { { WireWidth::W1, WireSide::Top, Direction::E }, true }, Sprite::WireTopOnEast_1 },
        { { { WireWidth::W1, WireSide::Top, Direction::N }, false }, Sprite::WireTopOffNorth_1 },
        { { { WireWidth::W1, WireSide::Top, Direction::S }, false }, Sprite::WireTopOffSouth_1 },
        { { { WireWidth::W1, WireSide::Top, Direction::W }, false }, Sprite::WireTopOffWest_1 },
        { { { WireWidth::W1, WireSide::Top, Direction::E }, false }, Sprite::WireTopOffEast_1 },
    };

    auto it = configs.find({ wire, value });
    if (it == configs.end())
        throw std::runtime_error("Wire configuration not found");
    return it->second;
}
