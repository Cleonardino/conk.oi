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

class Character{
    private:
    CharacterType type;
    bool has_moved;

    public:

    Character(CharacterType type_, bool has_moved_);
    CharacterType get_type() const;
    bool get_has_moved() const;
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
    Character character;

    public:

    Tile(TileType tile_type_, int owner_id_, bool has_wall_,
        BuildingType building_type_, Character character_);

    static Tile default_Tile();

    void update_tile(TileType tile_type_, int owner_id_, bool has_wall_,
        BuildingType building_type_, Character character_);

    TileType get_type() const;
    int get_owner() const;
    bool get_wall() const;
    BuildingType get_building() const;
    Character get_character() const;

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
    int get_height() const;
    int get_width() const;
    Tile get_Tile(coordinates location) const;
    void set_Tile(coordinates location, TileType tile_type, int owner_id,
        bool has_wall, BuildingType building_type, Character character);

    friend std::ostream& operator<<(std::ostream& os, const Map& map);
};

#endif