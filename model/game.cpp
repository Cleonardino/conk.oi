#include "game.hpp"

Province::Province(int owner_id_):
locations(std::set<coordinates>()),
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
std::set<coordinates> Province::get_locations() const{
    return locations;
}

// Add a location to the province
void Province::add_location(coordinates location){
    locations.insert(location);
    income++;
}

bool Province::does_contain(coordinates location) const{
    return (locations.find(location) != locations.end());
}


// << operator
std::ostream& operator<<(std::ostream& os, const Province& province){
    os << "==Province (Owner:" << province.get_owner() << " Gold:" << province.get_gold() <<
    " Income:" << province.get_income() << ")" << std::endl;
    for(coordinates location : province.get_locations()){
        os << ">>" << location.first << "," << location.second << std::endl;
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
Game::Game(Map map_, int active_player_id_, std::vector<Province> provinces_):
map(map_), active_player_id(active_player_id_), provinces(provinces_)
{
    selected_location = coordinates(-1,-1);
    update_select();
    update_provinces();
    // Get max player id + 1 from map to have the max_player_count
    max_player_count = 1;
    for(int i = 0; i < map.get_height(); i++){
        for(int j = 0; j < map.get_width(); j++){
            max_player_count = std::max(max_player_count,map.get_Tile(coordinates(i,j)).get_owner() + 1);
        }
    }
}

// Getter of game's map height
int Game::get_height() const{
    return map.get_height();
}

// Getter of game's map width
int Game::get_width() const{
    return map.get_width();
}

// Getter of max player count
int Game::get_max_player_count() const{
    return max_player_count;
}

// Get all neighbours location. Only count lands as neighbors.
std::vector<coordinates> Game::get_neighbours_locations(coordinates location) const{
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
    // for(int i = 0; i < map.get_height(); i++){
    //     for(int j = 0; j < map.get_width(); j++){
    //         if(map.get_Tile(coordinates(i,j)).get_character().get_type() != Empty &&
    //         map.get_Tile(coordinates(i,j)).get_character().get_type() != Bandit){
    //             // A standard unit is present, check if it's in a province
    //             if(treated.find(coordinates(i,j)) == treated.end()){
    //                 // Not treated -> not connected to a province
    //                     map.set_Tile(coordinates(i,j),Land,map.get_Tile(coordinates(i,j)).get_owner(),
    //                     false,Building(Wild),Character(Bandit,false));
    //             }
    //         }
    //     }
    // }
}

int Game::get_active_player_id() const{
    return active_player_id;
}

int Game::get_next_player_id() const{
    return active_player_id;
}

bool Game::do_display_panel() const{
    return true;
}

int Game::get_displayed_income() const{
    return 123;
}

int Game::get_displayed_gold() const{
    return 456;
}


bool does_wall_connect(Map map, coordinates location, coordinates adjacent_tile){
    return (map.get_Tile(adjacent_tile).get_wall() &&
    map.get_Tile(adjacent_tile).get_owner() == 
    map.get_Tile(location).get_owner());
}

// Compute all infos needed for displaying a tile
TileDisplayInfos Game::get_display_infos(coordinates location) const{
    // List of walls in the following order : {TL,TR,L,R,BL,BR}
    // Init wall list with has_wall value
    std::vector<bool> walls = std::vector<bool>(6,map.get_Tile(location).get_wall());
    bool selected = (location==selected_location);
    bool is_province_selected = province_selected[location.first][location.second];
    bool is_valid_destination = valid_destination[location.first][location.second];

    // Computing walls
    // Wall on left
    if(location.second > 0){
        // Not on left column
        walls[2] = walls[2] && !does_wall_connect(map,location,coordinates(location.first,location.second-1));
    }

    // Wall on right
    if(location.second < map.get_width() - 1){
        // Not on right column
        walls[3] = walls[3] && !does_wall_connect(map,location,coordinates(location.first,location.second+1));
    }

    if(location.first % 2 == 0){
        // Even row
        if(location.first > 0){
            // Not on top row
            if(location.second > 0){
                // Not on left column
                walls[0] = walls[0] && !does_wall_connect(map,location,coordinates(location.first-1,location.second-1));
            }
            walls[1] = walls[1] && !does_wall_connect(map,location,coordinates(location.first-1,location.second));
        }
        if(location.first < map.get_height() - 1){
            // Not on the bottom row
            if(location.second > 0){
                // Not on left column
                walls[4] = walls[4] && !does_wall_connect(map,location,coordinates(location.first+1,location.second-1));
            }
            walls[5] = walls[5] && !does_wall_connect(map,location,coordinates(location.first+1,location.second));
        }
    }else{
        // Odd row
        if(location.first > 0){
            // Not on top row
            if(location.second < map.get_width() - 1){ 
                // Not on right column
                walls[1] = walls[1] && !does_wall_connect(map,location,coordinates(location.first-1,location.second+1));
            }
            walls[0] = walls[0] && !does_wall_connect(map,location,coordinates(location.first-1,location.second));
        }
        if(location.first < map.get_height() - 1){
            // Not on the bottom row
            if(location.second < map.get_width() - 1){
                // Not on right column
                walls[5] = walls[5] && !does_wall_connect(map,location,coordinates(location.first+1,location.second+1));
            }
            walls[4] = walls[4] && !does_wall_connect(map,location,coordinates(location.first+1,location.second));
        }
    }

    // To complete
    return TileDisplayInfos(map.get_Tile(location), walls, selected, is_province_selected, is_valid_destination);
}

std::ostream& operator<<(std::ostream& os, const Game& game){
    os << "=====Game=====" << std::endl << "Selected Location: (" << game.selected_location.first <<
    "," << game.selected_location.second << ")" <<
    std::endl << game.map << std::endl << "===Provinces===" << std::endl;
    for(Province province : game.provinces){
        std::cout << province;
    }
    return os;
}

bool Game::is_destination_valid(coordinates destination) const{
    // If no character is selected,the character is sleeping or the
    // selected tile don't belong to the active player, no destination is marked as valid
    if(map.get_Tile(selected_location).get_owner() != active_player_id ||
        map.get_Tile(selected_location).get_character().get_type() == Empty ||
        map.get_Tile(selected_location).get_character().get_has_moved()){
        return false;
    }

    if(map.get_Tile(destination).get_owner() == map.get_Tile(selected_location).get_owner()){
        // Same owner, valid only if empty
        return (map.get_Tile(destination).get_character().get_type() == Empty &&
        map.get_Tile(destination).get_building().get_type() == Wild);
    }

    // Tile to attack, check if the power level is sufficient
    // Heroes can attack whatever they want
    if(map.get_Tile(selected_location).get_character().get_type() == Hero){
        return true;
    }
    // Check if power level is sufficient
    return get_power_level(destination)<get_singular_power_level(selected_location);
}

// Compute power level of a tile without checking its neighbours
int Game::get_singular_power_level(coordinates location) const{
    int character_power = 0;
    int building_power = 0;
    switch (map.get_Tile(location).get_character().get_type())
    {
    case Peasant:
        character_power = PEASANT_POWER;
        break;
    case Soldier:
        character_power = SOLDIER_POWER;
        break;
    case Knight:
        character_power = KNIGHT_POWER;
        break;
    case Hero:
        character_power = HERO_POWER;
        break;
    }
        switch (map.get_Tile(location).get_building().get_type())
    {
    case Town:
        character_power = TOWN_POWER;
        break;
    case Fortress:
        character_power = FORTRESS_POWER;
        break;
    }
    return std::max(character_power,building_power);
}

// Compute power level of a tile
int Game::get_power_level(coordinates location) const{
    int result = get_singular_power_level(location);
    for(coordinates neighbour : get_neighbours_locations(location)){
        if(map.get_Tile(neighbour).get_owner() == map.get_Tile(location).get_owner()){
            // If same owner, use protection
            result = std::max(result,get_singular_power_level(neighbour));
        }
    }
    return result;
}

// Update selected_province and valid_destination boolean matrixes.
void Game::update_select(){
    // Reset selection
    province_selected = std::vector<std::vector<bool>>(map.get_height(),std::vector<bool>(map.get_width(),false));
    valid_destination = std::vector<std::vector<bool>>(map.get_height(),std::vector<bool>(map.get_width(),false));
    std::vector<coordinates> tiles_selected = std::vector<coordinates>();
    // Compute new selection
    for(Province province : provinces){
        if(province.does_contain(selected_location)){
            // The selected province
            for(coordinates location : province.get_locations()){
                province_selected[location.first][location.second] = true;
                tiles_selected.push_back(location);
            }
        }
    }
    std::vector<coordinates> edges;
    // Get neighbours on selected province's edges
    for(coordinates location : tiles_selected){
        edges = get_neighbours_locations(location);
        for(coordinates edge : edges){
            valid_destination[edge.first][edge.second] = is_destination_valid(edge);
        }
    }
}

void Game::on_tile_click(coordinates location){
    if(location == selected_location){
        // Only Unselect
        selected_location = coordinates(-1,-1);
        update_select();
        return;
    }
    if(map.get_Tile(selected_location).get_character().get_type() != Empty){
        // A character is selected
        if(valid_destination[location.first][location.second]){
            // Characters always destroy buildings where they move
            bool has_wall = false;
            Building building = Building(Wild);
            Character character = map.get_Tile(selected_location).get_character();
            // Move the character
            if(map.get_Tile(location).get_owner() != active_player_id){
                // Exhausting character
                character.exhaust();
            }
            // Setting destination tile
            map.set_Tile(
                location,
                Land,
                active_player_id,
                has_wall,
                building,
                character
                );
            // Removing character from origin tile
            map.set_Tile(
                selected_location,
                Land,
                active_player_id,
                map.get_Tile(selected_location).get_wall(),
                map.get_Tile(selected_location).get_building(),
                Character(Empty,false)
                );
            selected_location = coordinates(-1,-1);
            // Updating provinces
            update_provinces();
        }
    }

    if(map.get_Tile(location).get_type() == Land){
        selected_location = location;
    }

    update_select();
}

void Game::on_end_turn(){
    
}


void Game::on_rewind(){
    
}

void Game::on_peasant_purchase(){

}

void Game::on_soldier_purchase(){

}

void Game::on_knight_purchase(){

}

void Game::on_hero_purchase(){

}

void Game::on_fortress_purchase(){

}
