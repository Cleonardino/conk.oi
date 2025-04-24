#include "game.hpp"

Province::Province(std::string name_, int owner_id_):
coordinates(std::vector<std::pair<int,int>>(0,std::pair<int,int>(-1,-1))),
name(name_), owner_id(owner_id_), gold(0)
{}