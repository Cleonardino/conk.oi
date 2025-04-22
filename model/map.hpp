#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <vector>

enum TileType {
    Forest,
    Ocean,
    Land
};

enum CharacterType {
    Empty,
    Peasant,
    Soldier,
    Knight
};

class Tile{
    private:
    TileType tile_type;
    int owner_id;
    bool has_wall;
    bool has_town;
    CharacterType character_type;

    public:

    Tile();

    Tile update_tile(TileType tile_type, int owner_id, bool has_wall,
    bool has_town, CharacterType character_type);

    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
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