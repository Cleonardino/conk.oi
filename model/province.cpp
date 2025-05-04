#include "province.hpp"


Province::Province(int owner_id_):
locations(std::set<coordinates>()),
owner_id(owner_id_), gold(0), income(0)
{}

// Add the amount gold to the province (can be negative)
int Province::add_gold(int amount){
    gold += amount;
    return gold;
}

// Getter method
int Province::get_gold() const{
    return gold;
}

// Getter method
int Province::get_owner() const{
    return owner_id;
}

// Getter method
int Province::get_income() const{
    return income;
}

// Getter method
std::set<coordinates> Province::get_locations() const{
    return locations;
}

// Add a location to the province
void Province::add_location(coordinates location){
    locations.insert(location);
    income++;
}

bool Province::does_contain(coordinates location) const{
    return (locations.find(location) != locations.end());
}


// << operator
std::ostream& operator<<(std::ostream& os, const Province& province){
    os << "==Province (Owner:" << province.get_owner() << " Gold:" << province.get_gold() <<
    " Income:" << province.get_income() << ")" << std::endl;
    for(coordinates location : province.get_locations()){
        os << ">>" << location.first << "," << location.second << std::endl;
    }
    return os;
}