#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "map.hpp"
#include "parser.hpp"

// A class consisting of a collection of tiles coordinates, a name, an owner id and a selected tag
class Province{
    std::vector<std::pair<int, int>> coordinates;
    std::string name;
    int owner_id;
};

// A class containing all the informations of the current game's state as well as the map
class Game{
    Map map;
    int active_player_id;
    std::vector<Province> provinces;
    std::pair<int, int> selected_coordinates;
};

#endif