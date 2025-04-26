#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "map.hpp"
#include "parser.hpp"

// A class consisting of a collection of tiles locations, and all informations relevant to a province.
class Province{
    private:
    std::vector<coordinates> locations;
    int owner_id;
    int gold;
    int income;

    public:
    Province(std::string name_, int owner_id);
    int add_gold(int amount);
    int get_gold();
    void add_location(coordinates location);

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
};

// A class containing all the informations of the current game's state as well as the map.
class Game{
    private:
    Map map;
    int active_player_id;
    std::vector<Province> provinces;
    coordinates selected_location;
    
    public:
    Game(Map map_, int active_player_id_, std::vector<Province> provinces_, coordinates selected_location_);
    std::vector<coordinates> get_neighbours_locations(coordinates location);
    void update_provinces();
    TileDisplayInfos get_display_infos(coordinates location) const;

};

#endif