#include "tiledisplayinfo.hpp"

// Class constructor
TileDisplayInfos::TileDisplayInfos(Tile tile_, std::vector<bool> walls_, bool selected_, bool province_selected_, bool valid_destination_):
tile(tile_),walls(walls_),selected(selected_),province_selected(province_selected_),valid_destination(valid_destination_)
{}

// Getter method
Tile TileDisplayInfos::get_Tile() const{
    return tile;
}

// Getter method
std::vector<bool> TileDisplayInfos::get_walls() const{
    return walls;
}

// Getter method
bool TileDisplayInfos::get_selected() const{
    return selected;
}

// Getter method
bool TileDisplayInfos::get_province_selected() const{
    return province_selected;
}

// Getter method
bool TileDisplayInfos::get_valid_destination() const{
    return valid_destination;
}