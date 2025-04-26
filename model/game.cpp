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



// Get all neighbours location. Only count lands as neighbors.
std::vector<coordinates> Game::get_neighbours_locations(coordinates location){
    std::vector<coordinates> result;
    // Tile on left
    if(location.second > 0){
        // Not on the left column
        if(map.get_Tile(coordinates(location.first,location.second-1)).get_type() == Land){
            // Adding tile on the left
            result.push_back(coordinates(location.first,location.second-1));
        }
    }
    // Tile on right
    if(location.second < map.get_width() - 1){
        // Not on the right column
        if(map.get_Tile(coordinates(location.first,location.second+1)).get_type() == Land){
            // Adding tile on the left
            result.push_back(coordinates(location.first,location.second+1));
        }
    }

    // Tile on top
    if(location.first > 0){
        // Not on the top row
        if(map.get_Tile(coordinates(location.first-1,location.second)).get_type() == Land){
            // Adding tile on the top
            result.push_back(coordinates(location.first-1,location.second));
        }
        if(location.first % 2 == 0){
            if(location.second > 0){
            // Even row, check tile on the top left
                if(map.get_Tile(coordinates(location.first-1,location.second-1)).get_type() == Land){
                    // Adding tile on the top left
                    result.push_back(coordinates(location.first-1,location.second-1));
                }
            }
        }else{
            if(location.second < map.get_width() - 1){
            // Odd row, check tile on the top right
                if(map.get_Tile(coordinates(location.first-1,location.second+1)).get_type() == Land){
                    // Adding tile on the top right
                    result.push_back(coordinates(location.first-1,location.second+1));
                }
            }
        }
    }
    // Tile on bottom
    if(location.first < map.get_height() - 1){
        // Not on the bottom row
        if(map.get_Tile(coordinates(location.first+1,location.second)).get_type() == Land){
            // Adding tile on the bottom
            result.push_back(coordinates(location.first+1,location.second));
        }
        if(location.first % 2 == 0){
            if(location.second > 0){
            // Even row, check tile on the bottom left
                if(map.get_Tile(coordinates(location.first+1,location.second-1)).get_type() == Land){
                    // Adding tile on the bottom left
                    result.push_back(coordinates(location.first+1,location.second-1));
                }
            }
        }else{
            if(location.second < map.get_width() - 1){
            // Odd row, check tile on the bottom right
                if(map.get_Tile(coordinates(location.first+1,location.second+1)).get_type() == Land){
                    // Adding tile on the bottom right
                    result.push_back(coordinates(location.first+1,location.second+1));
                }
            }
        }
    }
    return result;
}

// Compute provinces after a tile is changed
void Game::update_provinces(){
    // Reset province
    // Check for strayed units and turn them into bandits
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