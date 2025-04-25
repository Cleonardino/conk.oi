#include "parser.hpp"

// Split a string based on split_char
std::vector<std::string> split(const std::string& str, char split_char) {
    std::vector<std::string> result;
    std::string part;

    for (char ch : str) {
        if (ch == split_char) {
            result.push_back(part);
            part = "";
        } else {
            part += ch;
        }
    }

    result.push_back(part);
    return result;
}

// Parse a csv file into a map
Map parse_csv(std::string filepath) {
    // File extraction variables
    std::ifstream file(filepath);
    std::string line;
    std::vector<std::string> splitted_line;
    std::vector<std::string> splitted_tile;

    // Map data
    int height, width;

    // Tile data
    TileType tile_type;
    int owner_id;
    bool has_wall;
    BuildingType building_type;
    CharacterType character_type;

    if (file.is_open()) {
        // Getting first line with map size
        std::getline(file, line);
        splitted_line = split(line,';');

        height = std::stoi(splitted_line[0]);
        width = std::stoi(splitted_line[1]);

        Map result = Map(height,width);
        // Current tile's coordinates
        int cur_row = 0;
        int cur_column = 0;
        while(std::getline(file, line)){
            splitted_line = split(line,';');
            for(std::string tilestring : splitted_line){
                // Decomposing tilestring into a tile
                // tile_type
                switch (tilestring[0])
                {
                case ' ':
                    // Ocean
                    tile_type = Ocean;
                    owner_id = -1;
                    has_wall = false;
                    building_type = Wild;
                    character_type = Empty;
                    break;
                
                case '&':
                    // Forest
                    tile_type = Forest;
                    owner_id = -1;
                    has_wall = false;
                    building_type = Wild;
                    character_type = Empty;
                    break;
                
                default:
                    // Land
                    tile_type = Land;
                    splitted_tile = split(tilestring,',');
                    if(splitted_tile[1][0] == ' '){
                        // No owner
                        owner_id = -1;
                    }else{
                        // Owner
                        owner_id = std::stoi(splitted_tile[1]);
                    }
                    has_wall = (splitted_tile[2][0] == 'w');
                    switch (splitted_tile[3][0])
                    {
                    case 't':
                        // Town
                        building_type = Town;
                        break;
                    case 'f':
                        building_type = Fortress;
                        break;
                    default:
                        // Wild
                        building_type = Wild;
                        break;
                    }
                    switch (splitted_tile[4][0])
                    {
                    case 'p':
                        // Peasant
                        character_type = Peasant;
                        break;
                    case 's':
                        // Soldier
                        character_type = Soldier;
                        break;
                    case 'k':
                        // Knight
                        character_type = Knight;
                        break;
                    case 'h':
                        // Knight
                        character_type = Hero;
                        break;
                    default:
                        // Empty
                        character_type = Empty;
                        break;
                    }
                    break;
                }

                // Update tile
                result.set_Tile(coordinates(cur_row, cur_column), tile_type, owner_id,
                    has_wall, building_type, character_type);
                    
                cur_column++;
            }
            cur_row++;
            cur_column = 0;
        }
        file.close();
        return result;
    } else {
        std::cerr << "Error : " << filepath << "could not be opened" << std::endl;
        return Map(0,0);
    }

}

int main(){
    Map new_map = parse_csv("example_map.txt");
    std::cout << new_map;
    return 0;
}