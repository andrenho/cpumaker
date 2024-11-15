#ifndef POSITION_HH
#define POSITION_HH

struct Position {
    ssize_t x, y;
    bool operator==(Position const& other) const { return x == other.x && y == other.y; }
};

template<>
struct std::hash<Position> {
    std::size_t operator()(Position const& p) const noexcept
    {
        return (std::hash<ssize_t>{}(p.x) << 1) ^ std::hash<ssize_t>{}(p.y);
    }
};

//---------------------------------------

enum class Direction { N, S, W, E };

struct SubPosition {
    Position  pos;
    Direction dir;
};

template<>
struct std::hash<SubPosition> {
    std::size_t operator()(SubPosition const& sp) const noexcept
    {
        return (std::hash<Position>{}(sp.pos) << 1) ^ std::hash<Direction>{}(sp.dir);
    }
};

#endif //POSITION_HH
