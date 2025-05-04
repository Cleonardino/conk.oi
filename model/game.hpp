#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "map.hpp"
#include "parser.hpp"
#include "game_constants.hpp"
#include "province.hpp"
#include "tiledisplayinfo.hpp"

// A class containing all the informations of the current game's state as well as the map.
class Game{
    private:
    Map map;
    int active_player_id;
    int max_player_count;
    bool display_panel;
    int displayed_income;
    int displayed_gold;
    bool buying_mode;
    std::vector<Province> provinces;
    coordinates selected_location;
    std::vector<std::vector<bool>> province_selected;
    std::vector<std::vector<bool>> valid_destination;
    Tile cursor_infos;

    private:
    void update_provinces();
    void update_select();
    void update_select_for_placement();
    std::vector<coordinates> get_neighbours_locations(coordinates location) const;
    bool is_destination_valid(coordinates destination) const;
    int get_singular_power_level(coordinates location) const;
    int get_power_level(coordinates location) const;
    void move_character(coordinates source, coordinates destination);
    
    public:
    Game(Map map_, int active_player_id_, std::vector<Province> provinces_);
    int get_height() const;
    int get_width() const;
    int get_max_player_count() const;

    int get_active_player_id() const;
    int get_next_player_id() const;

    bool do_display_panel() const;
    int get_displayed_income() const;
    int get_displayed_gold() const;
    

    TileDisplayInfos get_cursor_infos() const;
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