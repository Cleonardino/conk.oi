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

enum BuildingType {
    Wild,
    Town,
    Fortress
};

class Tile{
    private:
    TileType tile_type;
    int owner_id;
    bool has_wall;
    BuildingType building_type;
    CharacterType character_type;

    public:

    Tile(TileType tile_type, int owner_id, bool has_wall,
        BuildingType building_type, CharacterType character_type);

    static Tile default_Tile();

    void update_tile(TileType tile_type, int owner_id, bool has_wall,
        BuildingType building_type, CharacterType character_type);

    TileType get_type();
    int get_owner();
    bool get_wall();
    BuildingType get_building();
    CharacterType get_character();

    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
};

class Map{
    private:
    std::vector<std::vector<Tile>> data;
    int height;
    int width;

    public:

    Map(int height_, int width_);
    Tile get_Tile(int row, int column);
    void set_Tile(int row, int column, TileType tile_type, int owner_id,
        bool has_wall, BuildingType building_type, CharacterType character_type);

    friend std::ostream& operator<<(std::ostream& os, const Map& map);
};

#endif