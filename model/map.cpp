#include "map.hpp"

Map::Map(int height, int width):
height(height), width(width), data(height, std::vector<Tile>(width, Tile()))
{}

Tile::Tile():
owner_id(-1), has_wall(false), character_type(-1)
{}