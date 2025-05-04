#ifndef GAME_UTILS_HPP
#define GAME_UTILS_HPP

#include "map.hpp"

// Return true if the two locations location and adjacent_tile have connected walls
bool does_wall_connect(Map map, coordinates location, coordinates adjacent_tile);

struct Cube {
    int x, y, z;
};

Cube coordinates_to_cube(coordinates location);
int cube_distance(const Cube& a, const Cube& b);
int hex_distance(coordinates location_a, coordinates location_b);
void print_debug(int value);

#endif