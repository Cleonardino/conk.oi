#include "map.hpp"

Map::Map(int height, int width):
height(height), width(width), data(height, std::vector<Tile>(width, Tile()))
{}

Tile::Tile():
owner_id(-1), has_wall(false), character_type(-1)
{}

// Used for debugging purpose and testing of game's logic
std::ostream& operator<<(std::ostream& os, const Map& map) {
        os << "===== MAP (" << map.height << "x" << map.width << ") =====";
        return os;
}