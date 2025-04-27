#include "game.hpp"

Province::Province(int owner_id_):
locations(std::vector<coordinates>()),
owner_id(owner_id_), gold(0), income(0)
{}

// Add the amount gold to the province (can be negative)
int Province::add_gold(int amount){
    gold += amount;
    return gold;
}

// Getter method
int Province::get_gold() const{
    return gold;
}

// Getter method
int Province::get_owner() const{
    return owner_id;
}

// Getter method
int Province::get_income() const{
    return income;
}

// Getter method
std::vector<coordinates> Province::get_locations() const{
    return locations;
}

void Province::add_location(coordinates location){
    locations.push_back(location);
    income++;
}

std::ostream& operator<<(std::ostream& os, const Province& province){
    os << "Province (Owner:" << province.get_owner() << " Gold:" << province.get_gold();
    os << " Income:" << province.get_income() << ")" << std::endl;
    for(coordinates location : province.get_locations()){
        os << location.first << "," << location.second << std::endl;
    }
    return os;
}

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


// Class constructor
Game::Game(Map map_, int active_player_id_, std::vector<Province> provinces_, coordinates selected_location_):
map(map_), active_player_id(active_player_id_), provinces(provinces_),selected_location(selected_location_)
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
    // Reset provinces
    provinces.clear();
    
    std::vector<coordinates> all_towns;
    std::stack<coordinates> to_treat;
    std::set<coordinates> treated;
    // All tiles containing a town in a stack
    for(int i = 0; i < map.get_height(); i++){
        for(int j = 0; j < map.get_width(); j++){
            if(map.get_Tile(coordinates(i,j)).get_building().get_type() == Town){
                all_towns.push_back(coordinates(i,j));
            }
        }
    }
    int current_owner_id = -1;
    int current_province_id = -1;
    for(coordinates town_location : all_towns){
        // Add town
        to_treat.push(town_location);
        if(treated.find(town_location) == treated.end()){
            // Town isn't part of a province, create a new one with owner set to the town's owner
            current_owner_id = map.get_Tile(coordinates(town_location)).get_owner();
            provinces.push_back(Province(current_owner_id));
            current_province_id ++;
        }
        // Finding connected component
        while (!to_treat.empty()) {
            coordinates current = to_treat.top();
            to_treat.pop();
            if(treated.find(current) == treated.end() && map.get_Tile(current).get_owner() == current_owner_id){
                // If not already visited and with the same owner
                treated.insert(current);
                provinces[current_province_id].add_location(current);
                std::cout << "Visiting: (" << current.first << ", " << current.second << ")\n";
                std::vector<coordinates> neighbours = get_neighbours_locations(current);
                for(coordinates neighbor : neighbours){
                    if(treated.find(neighbor) == treated.end()){
                        to_treat.push(neighbor);
                    }
                }
            }
        }
    }
    // Check for strayed units and turn them into bandits
    for(int i = 0; i < map.get_height(); i++){
        for(int j = 0; j < map.get_width(); j++){
            if(map.get_Tile(coordinates(i,j)).get_character().get_type() != Empty &&
            map.get_Tile(coordinates(i,j)).get_character().get_type() != Bandit){
                // A standard unit is present, check if it's in a province
                if(treated.find(coordinates(i,j)) == treated.end()){
                    // Not treated -> not connected to a province
                        map.set_Tile(coordinates(i,j),Land,map.get_Tile(coordinates(i,j)).get_owner(),
                        false,Building(Wild),Character(Bandit,false));
                }
            }
        }
    }
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

int main(){
    Map new_map = parse_csv("example_map.txt");
    std::cout << new_map;
    Game new_game = Game(new_map,0,std::vector<Province>(),coordinates(-1,-1));
    std::vector<coordinates> test = new_game.get_neighbours_locations(coordinates(1,1));
    for(coordinates c : test){
        std::cout << c.first << "," << c.second << std::endl;
    }
    new_game.update_provinces();
    for(Province t : new_game.test()){
        std::cout << t;
    }
    std::cout << new_game.map;
    return 0;
}