#ifndef HISTORY_HPP
#define HISTORY_HPP

#include "map.hpp"
#include "province.hpp"

class GameStamp{
    private:
    Map map;
    int active_player_id;

    public:
    GameStamp(Map map_, int active_player_id_);
    Map get_map();
    int get_active_player_id();
};

#endif