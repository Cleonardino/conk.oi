#include "game_utils.hpp"

// Return true if the two locations location and adjacent_tile have connected walls
bool does_wall_connect(Map map, coordinates location, coordinates adjacent_tile){
    return (map.get_Tile(adjacent_tile).get_wall() &&
    map.get_Tile(adjacent_tile).get_owner() == 
    map.get_Tile(location).get_owner());
}

Cube coordinates_to_cube(coordinates location){
    int x = location.second - (location.first - (location.first & 1)) / 2;
    int z = location.first;
    int y = -x - z;
    return {x, y, z};
}

int cube_distance(const Cube& a, const Cube& b){
    return std::max(abs(a.x - b.x), std::max(abs(a.y - b.y), abs(a.z - b.z)));
}

int hex_distance(coordinates location_a, coordinates location_b){
    Cube a = coordinates_to_cube(location_a);
    Cube b = coordinates_to_cube(location_b);
    return cube_distance(a, b);
}

int main(){
    std::cout << hex_distance(coordinates(2,0),coordinates(2,2)) << std::endl;
}