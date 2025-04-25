#include "game.hpp"

Province::Province(std::string name_, int owner_id_):
locations(std::vector<coordinates>(0,coordinates(-1,-1))),
owner_id(owner_id_), gold(0)
{}

// Add the amount gold to the province (can be negative)
int Province::add_gold(int amount){
    gold += amount;
    return gold;
}

int Province::get_gold(){
    return gold;
}

void Province::add_location(coordinates location){
    locations.push_back(location);
}
