#ifndef PROVINCE_HPP
#define PROVINCE_HPP

#include "map.hpp"
#include <iostream>
#include <stack>
#include <set>

// A class consisting of a collection of tiles locations, and all informations relevant to a province.
class Province{
    private:
    std::set<coordinates> locations;
    int owner_id;
    int gold;
    int income;

    public:
    Province(int owner_id);
    int get_gold() const;
    int get_owner() const;
    int get_income() const;
    std::set<coordinates> get_locations() const;
    bool does_contain(coordinates location) const;
    void add_location(coordinates location, Tile tile);

    friend std::ostream& operator<<(std::ostream& os, const Province& province);
};


#endif