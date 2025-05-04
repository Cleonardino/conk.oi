#include "gamestamp.hpp"

GameStamp::GameStamp(Map map_, int active_player_id_):
map(map_), active_player_id(active_player_id_)
{}

Map GameStamp::get_map(){
    return map;
}

int GameStamp::get_active_player_id(){
    return active_player_id;
}
