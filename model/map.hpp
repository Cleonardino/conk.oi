#ifndef MAP_CPP
#define MAP_CPP
#define MAP_HEIGHT 10
#define MAP_WIDTH 15

#include <iostream>
#include <vector>

class Tile{

};

class Map{
    private:
    std::vector<std::vector<Tile>> data;

    public:
    Map();
};

#endif