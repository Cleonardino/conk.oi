#include "game.hpp"



// Class constructor
Game::Game(Map map_, int active_player_id_, std::vector<Province> provinces_):
map(map_), active_player_id(active_player_id_), provinces(provinces_), cursor_infos(Tile::default_Tile()),
displayed_income(0), displayed_gold(0), display_panel(false), buying_mode(false), history(), finished(false)
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
    compute_next_player_id();
    // Computing walls
    for(int i = 0; i < map.get_height(); i++){
        for(int j = 0; j < map.get_width(); j++){
            if(map.get_Tile(coordinates(i,j)).get_building().get_type() != Wild){
                // Add walls on tile and neighbours
                set_wall(coordinates(i,j),true);
                for(coordinates location : get_neighbours_locations(coordinates(i,j))){
                    // Set wall to true if tile is a land and have same owner
                    set_wall(location,map.get_Tile(location).get_type() == Land &&
                             map.get_Tile(location).get_owner() == map.get_Tile(coordinates(i,j)).get_owner());
                }
            }
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

// Set wall value on tile
void Game::set_wall(coordinates location, bool value){
    map.set_Tile(
    location,
    map.get_Tile(location).get_type(),
    map.get_Tile(location).get_owner(),
    value,
    map.get_Tile(location).get_building(),
    map.get_Tile(location).get_character()
);
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
                provinces[current_province_id].add_location(current,map.get_Tile(current));
                std::vector<coordinates> neighbours = get_neighbours_locations(current);
                for(coordinates neighbor : neighbours){
                    if(treated.find(neighbor) == treated.end()){
                        to_treat.push(neighbor);
                    }
                }
            }
        }
    }
}

// Return indexes of the province the coordinates location belongs to, or -1 if a stranded tile
int Game::get_province(coordinates location) const{
    for(int i = 0; i < provinces.size(); i++){
        if(provinces[i].does_contain(location)){
            return i;
        }
    }
    return -1;
}

// Make strayed units go stray
void Game::compute_strays(){
    for(int i = 0; i < map.get_height(); i++){
        for(int j = 0; j < map.get_width(); j++){
            if(map.get_Tile(coordinates(i,j)).get_character().get_type() != Empty &
            map.get_Tile(coordinates(i,j)).get_character().get_type() != Empty){
                // Standard units, check if going rogue
                if(get_province(coordinates(i,j)) == -1){
                    // Stranded, go rogue
                    map.set_Tile(
                        coordinates(i,j),
                        Land,
                        -1,
                        false,
                        Building(Wild,0),
                        Character(Bandit,false)
                    );
                }
            }
        }
    }
}

// Reset selection state
void Game::reset_select(){
    bool buying_mode = false;
    selected_location = coordinates(-1,-1);
    cursor_infos = Tile::default_Tile();
    update_provinces();
    update_select();
}

// Save the game as a gamestamp and stack it on history
void Game::save_gamestamp(){
    history.push(GameStamp(map,active_player_id));
}

// Load the most recent (top of stack) gamestamp in history. If history is empty, do nothing
void Game::load_gamestamp(){
    if(history.empty()){
        return;
    }
    map = history.top().get_map();
    active_player_id = history.top().get_active_player_id();
    history.pop();
    compute_next_player_id();
    reset_select();
}

// Return true if the player with id id is alive
bool Game::is_player_alive(int id){
    // To complete
    return true;
}

// Set next player id to next alive player or -1 if is last player. Set finished to true if only one player remain
void Game::compute_next_player_id(){
    int current_id = (active_player_id + 1) % max_player_count;
    next_player_id = active_player_id;
    // Do a complete loop
    while(current_id != active_player_id && next_player_id == active_player_id){
        if(is_player_alive(current_id)){
            // Found another alive player, set next_player_id to it
            next_player_id = current_id;
        }
        // Increment
        current_id  = (current_id + 1) % max_player_count;
    }
    // If a whole loop was done without finding any other alive player, set finished to true
    finished = (next_player_id == active_player_id);
    // If the end of the loop was reached, i.e. if the next_id is smaller than active_player_id, next_player is bandits (-1)
    if(next_player_id < active_player_id){
        next_player_id = -1;
    }
}

int Game::get_active_player_id() const{
    return active_player_id;
}

int Game::get_next_player_id() const{
    return next_player_id;
}

bool Game::do_display_panel() const{
    return display_panel;
}

int Game::get_displayed_income() const{
    return displayed_income;
}

int Game::get_displayed_gold() const{
    return displayed_gold;
}

TileDisplayInfos Game::get_cursor_infos() const{
    return TileDisplayInfos(cursor_infos,std::vector(6,true),false,false,false);
}

// Compute all infos needed for displaying a tile
TileDisplayInfos Game::get_display_infos(coordinates location) const{
    // List of walls in the following order : {TL,TR,L,R,BL,BR}
    // Init wall list with has_wall value
    std::vector<bool> walls = std::vector<bool>(6,map.get_Tile(location).get_wall());
    bool selected = (location==selected_location);
    bool is_province_selected = province_selected[location.first][location.second];
    bool is_valid_destination = valid_destination[location.first][location.second];
    if(buying_mode){
        // When in buying mode, valid_destination is only inside the selected province when no character or buildings
        // are present. Additionnally, no tile appears selected
        selected = false;
        is_valid_destination = is_province_selected &&
        (map.get_Tile(location).get_character().get_type() == Empty) &&
        (map.get_Tile(location).get_building().get_type() == Wild);
    }
    

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

bool Game::is_possible_to_move(coordinates destination) const{
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
    bool no_province_selected = true;
    int selected_province_id = get_province(selected_location);
    if(selected_province_id == -1){
        displayed_gold = 0;
        displayed_income = 0;
        display_panel = false;
    }else{
        if(map.get_Tile(selected_location).get_owner() == active_player_id){
            displayed_gold = provinces[selected_province_id].get_gold();
            displayed_income = provinces[selected_province_id].get_income();
            display_panel = true;
        }else{
            displayed_gold = 0;
            displayed_income = 0;
            display_panel = false;
        }
        for(coordinates location : provinces[selected_province_id].get_locations()){
            province_selected[location.first][location.second] = true;
            tiles_selected.push_back(location);
        }
    }
    std::vector<coordinates> edges;
    // Get neighbours on selected province's edges
    for(coordinates location : tiles_selected){
        edges = get_neighbours_locations(location);
        for(coordinates edge : edges){
            valid_destination[edge.first][edge.second] = is_possible_to_move(edge);
        }
    }
}

void Game::pay_for(coordinates location, int amount){
    std::vector<coordinates> nearest_towns = std::vector<coordinates>();
    int remaining_amount = amount;
    int selected_province_id = get_province(selected_location);
    if(selected_province_id == -1){
        return;
    }
    // Retrieve towns ordered by distance
    for(coordinates province_tile : provinces[selected_province_id].get_locations()){
        if(map.get_Tile(province_tile).get_building().get_type() == Town){
            // Town of province, insert in vector with
            auto iter = nearest_towns.begin();
            while (iter != nearest_towns.end() && hex_distance(*iter,location) < hex_distance(province_tile,location)) {
                ++iter;
            }
            nearest_towns.insert(iter, province_tile);
        }
    }
    auto iter = nearest_towns.begin();
    while(iter != nearest_towns.end() && remaining_amount > 0){
        // Pay with closest town
        int amount_payed = std::min(map.get_Tile(*iter).get_building().get_gold(),remaining_amount);
        map.add_gold(*iter,-amount_payed);
        remaining_amount -= amount_payed;
        std::cout << amount_payed << std::endl;
        ++iter;
    }
}

// Make the unit located at location try to retreat
void Game::try_retreat(coordinates location){
    for(coordinates neighbour : get_neighbours_locations(location)){
        // Retreat on first valid neighbour
        if(
            map.get_Tile(neighbour).get_owner() == map.get_Tile(location).get_owner() &&
            map.get_Tile(neighbour).get_building().get_type() == Wild &&
            map.get_Tile(neighbour).get_character().get_type() == Empty
        ){
            // Retreat to neighbour
            map.set_Tile(
                neighbour,
                Land,
                map.get_Tile(neighbour).get_owner(),
                map.get_Tile(neighbour).get_wall(),
                Building(Wild,0),
                map.get_Tile(location).get_character()
            );
            return;
        }
    }
}

void Game::move_character(coordinates source, coordinates destination){
    bool has_wall = false;
    // Checking if a building belonging to the player is nearby, adding walls if true
    for(coordinates neighbour : get_neighbours_locations(destination)){
        has_wall = has_wall ||
        (map.get_Tile(neighbour).get_building().get_type() != Wild &&
        map.get_Tile(neighbour).get_owner() == active_player_id);
    }
    Character character = map.get_Tile(source).get_character();
    bool building_destroyed = false;
    // Move the character
    if(map.get_Tile(destination).get_owner() != active_player_id){
        // Exhausting character
        character.exhaust();
        // Checking if a building is being destroyed
        building_destroyed = (map.get_Tile(destination).get_building().get_type() != Wild);
        // Checking if a character inside walls is killed. If so, try to retreat
        if(
            map.get_Tile(destination).get_character().get_type() != Empty &&
            map.get_Tile(destination).get_wall()
        ){
            try_retreat(destination);
        }
    }
    // Setting destination tile. Characters always destroy buildings where they move
    map.set_Tile(
        destination,
        Land,
        active_player_id,
        has_wall,
        Building(Wild,0),
        character
        );
    // Removing character from origin tile
    map.set_Tile(
        source,
        Land,
        active_player_id,
        map.get_Tile(source).get_wall(),
        map.get_Tile(source).get_building(),
        Character(Empty,false)
        );
    // Destroy adjacent walls if needed
    if(building_destroyed){
        // Destroying a building. Check for adjacent walls to remove
        for(coordinates neighbour : get_neighbours_locations(destination)){
            // Checking if an ally building is in range, otherwise destroy wall
            bool has_wall = false;
            for(coordinates adj_to_neighbour : get_neighbours_locations(neighbour)){
                has_wall = has_wall ||
                (map.get_Tile(adj_to_neighbour).get_building().get_type() != Wild &&
                map.get_Tile(adj_to_neighbour).get_owner() == map.get_Tile(neighbour).get_owner());
            }
            set_wall(neighbour, has_wall);
        }
    }
    // Updating provinces
    update_provinces();
    // Update next player id
    compute_next_player_id();
}

void Game::place_unit(coordinates location){
    // Place unit
    map.set_Tile(
        location,
        Land,
        active_player_id,
        map.get_Tile(location).get_wall(),
        cursor_infos.get_building(),
        cursor_infos.get_character()
    );
    // Remove gold
    int to_pay = 0;
    to_pay += cursor_infos.get_character().get_cost();
    to_pay += cursor_infos.get_building().get_cost();
    pay_for(location,to_pay);
    
    if(cursor_infos.get_building().get_type() == Fortress){
        set_wall(location,true);
        // If the unit is a fortress, add walls on neighbours
        for(coordinates neighbour : get_neighbours_locations(location)){
            if(map.get_Tile(neighbour).get_owner() == active_player_id){
                set_wall(neighbour,true);
            }
        }
    }
}

void Game::on_tile_click(coordinates location){
    // Checking if a character is being bought
    if(buying_mode){
        buying_mode = false;
        // Something is being buyed, trying to place it
        if(province_selected[location.first][location.second] &&
        (map.get_Tile(location).get_character().get_type() == Empty) &&
        (map.get_Tile(location).get_building().get_type() == Wild)){
            // Unit can be placed
            save_gamestamp();
            place_unit(location);
        }
        cursor_infos = Tile::default_Tile();
        // Calculate province upkeep and gold again
        update_provinces();
        update_select();
        return;
    }
    // Checking if unselecting a tile
    if(location == selected_location){
        // Only Unselect
        selected_location = coordinates(-1,-1);
        update_select();
        return;
    }
    // Checking if moving a character
    if(map.get_Tile(selected_location).get_character().get_type() != Empty){
        // A character is selected
        if(valid_destination[location.first][location.second]){
            // Move character
            save_gamestamp();
            move_character(selected_location,location);
            selected_location = coordinates(-1,-1);
            update_select();
            return;
        }
    }

    // Classic select
    if(map.get_Tile(location).get_type() == Land){
        selected_location = location;
    }

    update_select();
}

// When pressing the end turn button
void Game::on_end_turn(){
    active_player_id = next_player_id;
    compute_next_player_id();
}

// When pressing the rewind button
void Game::on_rewind(){
    load_gamestamp();
}

// Event function. Called when pressing the peasant purchase button
void Game::on_peasant_purchase(){
    if(cursor_infos.get_character().get_type() == Peasant){
        // Unselect
        buying_mode = false;
        cursor_infos = Tile::default_Tile();
    }else{
        buying_mode = true;
        cursor_infos = Tile(Land,active_player_id,false,Building(Wild,0),Character(Peasant,true));
    }
}

// Event function. Called when pressing the soldier purchase button
void Game::on_soldier_purchase(){
    if(cursor_infos.get_character().get_type() == Soldier){
        // Unselect
        buying_mode = false;
        cursor_infos = Tile::default_Tile();
    }else{
        buying_mode = true;
        cursor_infos = Tile(Land,active_player_id,false,Building(Wild,0),Character(Soldier,true));
    }
}

// Event function. Called when pressing the knight purchase button
void Game::on_knight_purchase(){
    if(cursor_infos.get_character().get_type() == Knight){
        // Unselect
        buying_mode = false;
        cursor_infos = Tile::default_Tile();
    }else{
        buying_mode = true;
        cursor_infos = Tile(Land,active_player_id,false,Building(Wild,0),Character(Knight,true));
    }
}

// Event function. Called when pressing the hero purchase button
void Game::on_hero_purchase(){
    if(cursor_infos.get_character().get_type() == Hero){
        // Unselect
        buying_mode = false;
        cursor_infos = Tile::default_Tile();
    }else{
        buying_mode = true;
        cursor_infos = Tile(Land,active_player_id,false,Building(Wild,0),Character(Hero,true));
    }
}

// Event function. Called when pressing the fortress purchase button
void Game::on_fortress_purchase(){
    if(cursor_infos.get_building().get_type() == Fortress){
        // Unselect
        buying_mode = false;
        cursor_infos = Tile::default_Tile();
    }else{
        buying_mode = true;
        cursor_infos = Tile(Land,active_player_id,false,Building(Fortress,0),Character(Empty,false));
    }
}
