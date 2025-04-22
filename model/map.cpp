#include "map.hpp"

Tile::Tile():
owner_id(-1), has_wall(false), character_type(Empty), tile_type(Land)
{}

// Used for debugging purpose and testing of game's logic
std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    switch (tile.tile_type)
    {
    case Ocean:
        os << "     " << std::endl;
        break;
    
    case Forest:
        os << "~~~~~" << std::endl;
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

        // Print town
        if(tile.has_town){
            os << "T" << std::endl;
        }else{
            os << " " << std::endl;
        }

        // Print wall
        if(tile.has_wall){
            os << "|" << std::endl;
        }else{
            os << " " << std::endl;
        }
    }
}

Map::Map(int height, int width):
height(height), width(width), data(height, std::vector<Tile>(width, Tile()))
{}

// Used for debugging purpose and testing of game's logic
std::ostream& operator<<(std::ostream& os, const Map& map) {
        os << "===== MAP (" << map.height << "x" << map.width << ") =====" << std::endl;
        for(auto line : map.data){
            for(auto tile : line){
                os << tile;
            }
            os << std::endl;
        }
        return os;
}