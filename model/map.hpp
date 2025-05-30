#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <vector>
#include "game_constants.hpp"

using coordinates = std::pair<int,int>;

enum TileType {
    Ocean,
    Land,
    Forest
};

enum CharacterType {
    Empty,
    Bandit,
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
    void exhaust();
    void refresh();
    int get_cost() const;
    int get_upkeep() const;
    void upgrade();
};

enum BuildingType {
    Wild,
    Town,
    Fortress
};

class Building{
    private:
    BuildingType type;
    int gold;

    public:
    Building(BuildingType type_, int gold_);
    BuildingType get_type() const;
    int get_gold() const;
    void add_gold(int amount);
    int get_cost();
    int get_upkeep();
};

// A class that represent all the data associated with a tile.
class Tile{
    private:
    TileType tile_type;
    int owner_id;
    bool has_wall;
    Building building;
    Character character;

    public:

    Tile(TileType tile_type_, int owner_id_, bool has_wall_,
        Building building_, Character character_);

    static Tile default_Tile();

    void update_tile(TileType tile_type_, int owner_id_, bool has_wall_,
        Building building_, Character character_);

    TileType get_type() const;
    int get_owner() const;
    bool get_wall() const;
    Building get_building() const;
    Character get_character() const;
    void add_gold(int amount);
    void refresh();

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
        bool has_wall, Building building, Character character);
    void add_gold(coordinates location, int amount);
    void refresh(coordinates location);
    friend std::ostream& operator<<(std::ostream& os, const Map& map);
};

#endif