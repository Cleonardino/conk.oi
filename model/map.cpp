#include "map.hpp"

// Class constructor
Character::Character(CharacterType type_, bool has_moved_):
type(type_), has_moved(has_moved_)
{}

// Return type of character
CharacterType Character::get_type() const {
    return type;
}

// Return true if the character has already moved
bool Character::get_has_moved() const {
    return has_moved;
}

// Class constructor
Tile::Tile(TileType tile_type_, int owner_id_, bool has_wall_,
BuildingType building_type_, Character character_):
    tile_type(tile_type_), owner_id(owner_id_), has_wall(has_wall_),
    building_type(building_type_), character(character_)
{}

// The default Tile, used in map initialisation
Tile Tile::default_Tile(){
    return Tile(Ocean,-1,false,Wild,Character(Empty,false));
}

// Update the Tile parameters
void Tile::update_tile(TileType tile_type_, int owner_id_, bool has_wall_,
    BuildingType building_type_, Character character_){
    tile_type = tile_type_;
    owner_id = owner_id_;
    has_wall = has_wall_;
    building_type = building_type_;
    character = character_;
}

// Getter functions
TileType Tile::get_type() const {
    return tile_type;
}
int Tile::get_owner() const {
    return owner_id;
}
bool Tile::get_wall() const {
    return has_wall;
}
BuildingType Tile::get_building() const {
    return building_type;
}
Character Tile::get_character() const {
    return character;
}

// Used for debugging purpose and testing of game's logic
std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    switch (tile.tile_type)
    {
    case Ocean:
        os << "     ";
        break;
    
    case Forest:
        os << "&&&&&";
        break;
    
    case Land:
        // Print wall
        if(tile.has_wall){
            os << "|";
        }else{
            os << " ";
        }

        // Print color
        switch (tile.owner_id)
        {
        case -1:
            // No owner
            os << " ";
            break;
        
        case 0:
            // First player
            os << "0";
            break;
        
        case 1:
            // Second player
            os << "1";
            break;
        
        case 2:
            // Third player
            os << "2";
            break;
        
        default:
            break;
        }

        // Print character
        switch (tile.character.get_type())
        {
        case Empty:
            // No character
            os << " ";
            break;
        
        case Peasant:
            os << "-";
            break;
        
        case Soldier:
            os << "=";
            break;
        
        case Knight:
            os << "#";
            break;
        
        case Hero:
            os << "@";
            break;
        }

        // Print building
        switch (tile.building_type)
        {
        case Wild:
            os << " ";
            break;

        case Town:
            os << "T";
            break;

        case Fortress:
            os << "F";
            break;
        }

        // Print wall
        if(tile.has_wall){
            os << "|";
        }else{
            os << " ";
        }
    }
    return os;
}

// Class constructor
Map::Map(int height_, int width_):
height(height_), width(width_), data(height_, std::vector<Tile>(width_, Tile::default_Tile()))
{}

// Get map height
int Map::get_height() const{
    return height;
}

// Get map width
int Map::get_width() const{
    return width;
}

// Get a Tile based on its coordinates
Tile Map::get_Tile(coordinates location) const {
    return data[location.first][location.second];
}

// Set a Tile based on its coordinates
void Map::set_Tile(coordinates location, TileType tile_type, int owner_id,
    bool has_wall, BuildingType building_type, Character character){
    data[location.first][location.second].update_tile(tile_type,owner_id,has_wall,building_type,character);
}

// Used for debugging purpose and testing of game's logic
std::ostream& operator<<(std::ostream& os, const Map& map) {
    os << "===== MAP (" << map.height << "x" << map.width << ") =====" << std::endl;
    bool even_line_nb = true;
    for(auto line : map.data){
        for(auto tile : line){
            os << tile << " ";
        }
        os << std::endl << std::endl;
        even_line_nb = !even_line_nb;
        if(!even_line_nb){
            os << "   ";
        }
    }
    return os;
}