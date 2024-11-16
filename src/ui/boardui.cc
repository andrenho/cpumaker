#include "boardui.hh"

static constexpr int TILE_SIZE = 16;

void BoardUI::load_resources(SDL_Renderer* ren)
{
    spritesheet_.load(ren);
}

//
// EVENTS
//

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

        case SDL_KEYDOWN: {
            auto tile = mouse_tile();
            if (tile && e->key.repeat == 0) {
                auto [pos, dir] = *tile;
                switch (e->key.keysym.sym) {
                    case SDLK_n:
                        board_->components[pos] = Element { Element::NPN };
                        break;
                    case SDLK_p:
                        board_->components[pos] = Element { Element::PNP };
                        break;
                    case SDLK_b:
                        board_->components[pos] = Element { Element::BUTTON };
                        break;
                    case SDLK_l:
                        board_->components[pos] = Element { Element::LED };
                        break;
                    case SDLK_1:
                        drawing_wire_ = { pos, WireWidth::W1, WireSide::Top };
                        break;
                    default: break;
                }
            }
            break;
        }

        default: break;
    }
}

std::optional<SubPosition> BoardUI::mouse_tile() const
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    float tx = (((float) x / zoom_) - (float) rel_x_) / (float) TILE_SIZE;
    float ty = (((float) y / zoom_) - (float) rel_y_) / (float) TILE_SIZE;

    // TODO - direction ?

    if (tx >= 0 && ty >= 0 && tx < board_->w() && ty < board_->h()) {
        return SubPosition { { (ssize_t) tx, (ssize_t) ty }, Direction::N };
    }

    return {};
}

//
// DRAW
//

void BoardUI::draw(SDL_Renderer* ren) const
{
    SDL_RenderSetScale(ren, zoom_, zoom_);

    // draw board
    draw_board_borders(ren);
    for (ssize_t x = 0; x < board_->w(); ++x) {
        for (ssize_t y = 0; y < board_->h(); ++y) {
            draw_tile(ren, x, y);
        }
    }

    // draw temporary tile
    auto o_mouse_tile = mouse_tile();
    if (drawing_wire_ && o_mouse_tile)
        draw_temporary_wire(ren, *drawing_wire_, o_mouse_tile->pos);

    SDL_RenderSetScale(ren, 1.f, 1.f);
}

void BoardUI::draw_board_borders(SDL_Renderer* ren) const
{
    auto tiles = BoardSpriteSheet::board_sprites();

    draw_icon(ren, tiles.top_left, -2, -2);
    draw_icon(ren, tiles.top_right, board_->w(), -2);
    draw_icon(ren, tiles.bottom_left, -2, board_->h());
    draw_icon(ren, tiles.bottom_right, board_->w(), board_->h());

    for (ssize_t x = 0; x < board_->w(); ++x) {
        draw_icon(ren, tiles.top, x, -2);
        draw_icon(ren, tiles.bottom, x, board_->h());
    }

    for (ssize_t y = 0; y < board_->h(); ++y) {
        draw_icon(ren, tiles.left, -2, y);
        draw_icon(ren, tiles.right, board_->w(), y);
    }
}

void BoardUI::draw_tile(SDL_Renderer* ren, ssize_t x, ssize_t y) const
{
    draw_icon(ren, BoardSpriteSheet::board_sprites().inner, x, y);

    auto it = board_->components.find({ x, y });
    if (it != board_->components.end())
        for (BoardSpriteSheet::Sprite const& sprite: BoardSpriteSheet::component_sprites(it->second))
            draw_icon(ren, sprite, x, y);
}

void BoardUI::draw_temporary_wire(SDL_Renderer* ren, TempWire const& temp_wire, Position const& end) const
{
    for (SubPosition const& sp: Board::route_wire(temp_wire.start_pos, end)) {
        WireConfiguration wire { .width = temp_wire.width, .side = temp_wire.side, .dir = sp.dir };
        draw_icon(ren, BoardSpriteSheet::wire_sprite(wire, false), sp.pos.x, sp.pos.y, true);
    }
}

void BoardUI::draw_icon(SDL_Renderer* ren, BoardSpriteSheet::Sprite icon, ssize_t x, ssize_t y, bool semitransparent) const
{
    auto const& r = BoardSpriteSheet::coordinate(icon);
    SDL_Rect src { .x = r.x * TILE_SIZE, .y = r.y * TILE_SIZE, .w = r.w * TILE_SIZE, .h = r.h * TILE_SIZE };
    SDL_Rect dest = { .x = (int) (rel_x_ + (x * TILE_SIZE)), .y = (int) (rel_y_ + (y * TILE_SIZE)), .w = src.w, .h = src.h };
    SDL_SetTextureAlphaMod(spritesheet_.texture(), semitransparent ? 128 : 255);
    SDL_RenderCopy(ren, spritesheet_.texture(), &src, &dest);
}

