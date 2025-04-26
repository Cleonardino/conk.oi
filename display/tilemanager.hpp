#ifndef TILEMANAGER_HPP
#define TILEMANAGER_HPP
#include "display.hpp"
#include "texturemanager.hpp"
#include "constants.hpp"
#include <string>

struct Color_RGB
{
    int r;
    int g;
    int b;
};

class TileManager
{    
public:
    static SDL_Texture* LoadTile(const int tile_id, SDL_Renderer* renderer);
    static SDL_Texture* LoadTileforPlayer(const int player_id, const int player_number, SDL_Renderer* renderer);
    static Color_RGB GenerateColor(const int player_id, const int player_number);
};
#endif