#ifndef TILEMANAGER_HPP
#define TILEMANAGER_HPP
#include "display.hpp"
#include "texturemanager.hpp"
#include "constants.hpp"
#include "../model/game.hpp"
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
    static void DrawTile(SDL_Renderer* renderer, std::map<int, std::map<int, TexturePtr>> textures, SDL_Rect *dest, TileDisplayInfos to_draw, const int player_id);
    static TexturePtr LoadTile(const int tile_id, SDL_Renderer* renderer);
    static TexturePtr LoadTileforPlayer(const int player_id, const int player_number, SDL_Renderer* renderer, const char* filename);
    static Color_RGB GenerateColor(const int player_id, const int player_number);
};
#endif