#ifndef TILEDISPLAYINFO_HPP
#define TILEDISPLAYINFO_HPP
#include "map.hpp"

// A class containing all processed data of a tile that is needed to display it.
class TileDisplayInfos{
    private:
    Tile tile;
    std::vector<bool> walls;
    bool selected;
    bool province_selected;
    bool valid_destination;

    public:
    TileDisplayInfos(Tile tile_, std::vector<bool> walls_, bool selected_, bool province_selected_, bool valid_destination);
    Tile get_Tile() const;
    std::vector<bool> get_walls() const;
    bool get_selected() const;
    bool get_province_selected() const;
    bool get_valid_destination() const;
};

#endif