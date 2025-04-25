#include "game.hpp"

Province::Province(std::string name_, int owner_id_):
locations(std::vector<coordinates>(0,coordinates(-1,-1))),
owner_id(owner_id_), gold(0)
{}

// Add the amount gold to the province (can be negative)
int Province::add_gold(int amount){
    gold += amount;
    return gold;
}

int Province::get_gold(){
    return gold;
}

void Province::add_location(coordinates location){
    locations.push_back(location);
}

TileDisplayInfos::TileDisplayInfos(Tile tile_, std::vector<bool> walls_, bool selected_, bool province_selected_, bool valid_destination_):
tile(tile_),walls(walls_),selected(selected_),province_selected(province_selected_),valid_destination(valid_destination_)
{}

// Get all neighbours location. If neighbour does not exist or is not a land, the coordinates will be (-1,-1)
std::vector<coordinates> Game::get_neighbours_locations(coordinates location){
    // To do
}

// Compute provinces after a tile is changed
void Game::update_provinces(){
    // Flood province
    // All tiles in a list
    // Take first tile, erase it from list and mark it with an id. Repeat for all neighbors
    // Next province id, next tile that is in list.
}

// Compute all infos needed for displaying a tile
TileDisplayInfos Game::get_display_infos(coordinates location) const{
    std::vector<bool> walls = std::vector<bool>(6,false);
    bool selected = false;
    bool province_selected = false;
    bool valid_destination = false;
    // To complete
    return TileDisplayInfos(map.get_Tile(location), walls, selected, province_selected, valid_destination);
}