#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <vector>

class Tile{
    private:
    int owner_id;
    bool has_wall;
    int character_type;

    public:
    Tile();
};

class Map{
    private:
    std::vector<std::vector<Tile>> data;
    int height;
    int width;

    public:
    Map(int height, int width);
    friend std::ostream& operator<<(std::ostream& os, const Map& map);
};

#endif