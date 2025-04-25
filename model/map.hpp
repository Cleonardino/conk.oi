#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <vector>

using coordinates = std::pair<int,int>;

enum TileType {
    Ocean,
    Land,
    Forest
};

enum CharacterType {
    Empty,
    Peasant,
    Soldier,
    Knight,
    Hero
};

enum BuildingType {
    Wild,
    Town,
    Fortress
};

// A class that represent all the data associated with a tile.
class Tile{
    private:
    TileType tile_type;
    int owner_id;
    bool has_wall;
    BuildingType building_type;
    CharacterType character_type;

    public:

    Tile(TileType tile_type_, int owner_id_, bool has_wall_,
        BuildingType building_type_, CharacterType character_type_);

    static Tile default_Tile();

    void update_tile(TileType tile_type_, int owner_id_, bool has_wall_,
        BuildingType building_type_, CharacterType character_type_);

    TileType get_type();
    int get_owner();
    bool get_wall();
    BuildingType get_building();
    CharacterType get_character();

    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
};

// A class that represents the matrix of Tiles that makes the map.
class Map{
    private:
    std::vector<std::vector<Tile>> data;
    int height;
    int width;

    public:

    Map(int height_, int width_);
    Tile get_Tile(coordinates location);
    void set_Tile(coordinates location, TileType tile_type, int owner_id,
        bool has_wall, BuildingType building_type, CharacterType character_type);

    friend std::ostream& operator<<(std::ostream& os, const Map& map);
};

#endif