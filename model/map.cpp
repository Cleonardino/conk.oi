#include "map.hpp"

// Class constructor
Tile::Tile(TileType tile_type, int owner_id, bool has_wall,
BuildingType building_type, CharacterType character_type):
    tile_type(tile_type), owner_id(owner_id), has_wall(has_wall),
    building_type(building_type), character_type(character_type)
{}

// The default Tile, used in map initialisation
Tile Tile::default_Tile(){
    return Tile(Ocean,-1,false,Wild,Empty);
}

// Update the Tile parameters
void Tile::update_tile(TileType tile_type, int owner_id, bool has_wall,
    BuildingType building_type, CharacterType character_type){
    tile_type = tile_type;
    owner_id = owner_id;
    has_wall = has_wall;
    building_type = building_type;
    character_type = character_type;
}

// Getter functions
TileType Tile::get_type(){
    return tile_type;
}
int Tile::get_owner(){
    return owner_id;
}
bool Tile::get_wall(){
    return has_wall;
}
BuildingType Tile::get_building(){
    return building_type;
}
CharacterType Tile::get_character(){
    return character_type;
}

// Used for debugging purpose and testing of game's logic
std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    switch (tile.tile_type)
    {
    case Ocean:
        os << "     " << std::endl;
        break;
    
    case Forest:
        os << "&&&&&" << std::endl;
        break;
    
    case Land:
        // Print wall
        if(tile.has_wall){
            os << "|" << std::endl;
        }else{
            os << " " << std::endl;
        }

        // Print color
        switch (tile.owner_id)
        {
        case -1:
            // No owner
            os << " " << std::endl;
            break;
        
        case 0:
            // Player
            os << "P" << std::endl;
            break;
        
        case 1:
            // First enemy
            os << "1" << std::endl;
            break;
        
        case 2:
            // Second enemy
            os << "2" << std::endl;
            break;
        
        default:
            break;
        }

        // Print character
        switch (tile.character_type)
        {
        case Empty:
            // No character
            os << " " << std::endl;
            break;
        
        case Peasant:
            os << "-" << std::endl;
            break;
        
        case Soldier:
            os << "=" << std::endl;
            break;
        
        case Knight:
            os << "#" << std::endl;
            break;
        
        default:
            break;
        }

        // Print building
        switch (tile.building_type)
        {
        case Wild:
            os << " " << std::endl;
            break;

        case Town:
            os << "T" << std::endl;
            break;

        case Fortress:
            os << "F" << std::endl;
            break;
        }

        // Print wall
        if(tile.has_wall){
            os << "|" << std::endl;
        }else{
            os << " " << std::endl;
        }
    }
    return os;
}

// Class constructor
Map::Map(int height_, int width_):
height(height_), width(width_), data(height_, std::vector<Tile>(width_, Tile::default_Tile()))
{}

// Get a Tile based on its coordinates
Tile Map::get_Tile(int row, int column){
    return data[row][column];
}

// Set a Tile based on its coordinates
void Map::set_Tile(int row, int column, TileType tile_type, int owner_id,
    bool has_wall, BuildingType building_type, CharacterType character_type){
    data[row][column].update_tile(tile_type,owner_id,has_wall,building_type,character_type);
}

// Used for debugging purpose and testing of game's logic
std::ostream& operator<<(std::ostream& os, const Map& map) {
    os << "===== MAP (" << map.height << "x" << map.width << ") =====" << std::endl;
    for(auto line : map.data){
        for(auto tile : line){
            os << tile;
        }
        os << std::endl << std::endl;
    }
    return os;
}