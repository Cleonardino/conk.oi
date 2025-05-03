#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <stack>
#include <set>
#include "map.hpp"
#include "parser.hpp"
#include "game_constants.hpp"

// A class consisting of a collection of tiles locations, and all informations relevant to a province.
class Province{
    private:
    std::set<coordinates> locations;
    int owner_id;
    int gold;
    int income;

    public:
    Province(int owner_id);
    int add_gold(int amount);
    int get_gold() const;
    int get_owner() const;
    int get_income() const;
    std::set<coordinates> get_locations() const;
    bool does_contain(coordinates location) const;
    void add_location(coordinates location);

    friend std::ostream& operator<<(std::ostream& os, const Province& province);
};

// A class containing all processed data of a tile that is needed to display it.
class TileDisplayInfos{
    private:
    Tile tile;
    std::vector<bool> walls;
    bool selected;
    bool province_selected;
    bool valid_destination;

    public:
    TileDisplayInfos(Tile tile_, std::vector<bool> walls_, bool selected_, bool province_selected_, bool valid_destination);
    Tile get_Tile() const;
    std::vector<bool> get_walls() const;
    bool get_selected() const;
    bool get_province_selected() const;
    bool get_valid_destination() const;
};

// A class containing all the informations of the current game's state as well as the map.
class Game{
    private:
    Map map;
    int active_player_id;
    int max_player_count;
    std::vector<Province> provinces;
    coordinates selected_location;
    std::vector<std::vector<bool>> province_selected;
    std::vector<std::vector<bool>> valid_destination;

    private:
    void update_provinces();
    void update_select();
    std::vector<coordinates> get_neighbours_locations(coordinates location) const;
    bool is_destination_valid(coordinates destination) const;
    int get_singular_power_level(coordinates location) const;
    int get_power_level(coordinates location) const;
    
    public:
    Game(Map map_, int active_player_id_, std::vector<Province> provinces_);
    int get_height() const;
    int get_width() const;
    int get_max_player_count() const;

    int get_active_player_id() const;
    int get_next_player_id() const;
    
    TileDisplayInfos get_display_infos(coordinates location) const;
    void on_tile_click(coordinates location);
    void on_end_turn();
    void on_rewind();
    void on_peasant_purchase();
    void on_soldier_purchase();
    void on_knight_purchase();
    void on_hero_purchase();
    void on_fortress_purchase();
    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};

#endif