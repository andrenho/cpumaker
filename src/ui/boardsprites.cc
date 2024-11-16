#include "boardsprites.hh"
#include "boardsprites.hh"

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
    SDL_FreeSurface(sf);
}

BoardSpriteSheet::Coordinate const& BoardSpriteSheet::coordinate(Sprite sprites)
{
    return coordinates[(size_t) sprites];
}

BoardSpriteSheet::BoardTile const& BoardSpriteSheet::board_tiles()
{
    using enum Sprite;
    constexpr static BoardTile board_tile = {
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

std::vector<BoardSpriteSheet::Sprite> BoardSpriteSheet::sprite(Component const& component)
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
