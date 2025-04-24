#include "game.hpp"

Province::Province(std::string name_, int owner_id_):
locations(std::vector<coordinates>(0,coordinates(-1,-1))),
owner_id(owner_id_), gold(0)
{}