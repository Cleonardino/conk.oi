#include "tilemanager.hpp"

SDL_Texture* TileManager::LoadTile(const int tile_id, SDL_Renderer* renderer)
{
    std::map<int, const char*> id_to_tilename { {BASE_TILE, "../art/tiles/base_tile.png"},
                                                {FOREST_TILE, "../art/tiles/forest.png"},
                                                {OCEAN_TILE, "../art/tiles/ocean.png"}};
    SDL_Texture* tile_tex = TextureManager::LoadTexture(id_to_tilename[tile_id], renderer);

    return tile_tex;
}

Color_RGB TileManager::GenerateColor(const int player_id, const int player_number)
{
    float h = (360.0f / player_number) * player_id;
    float s = 0.8f;
    float v = 0.9f;

    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    float m = v - c;

    float rf, gf, bf;

    if (h < 60) { rf = c; gf = x; bf = 0; }
    else if (h < 120) { rf = x; gf = c; bf = 0; }
    else if (h < 180) { rf = 0; gf = c; bf = x; }
    else if (h < 240) { rf = 0; gf = x; bf = c; }
    else if (h < 300) { rf = x; gf = 0; bf = c; }
    else { rf = c; gf = 0; bf = x; }

    Color_RGB color = {
        (int)((rf + m) * 255),
        (int)((gf + m) * 255),
        (int)((bf + m) * 255)
    };
    return color;
}

SDL_Texture* TileManager::LoadTileforPlayer(const int player_id, const int player_number, SDL_Renderer* renderer)
{
    SDL_Surface* image = IMG_Load("../art/tiles/land.png");
    Color_RGB color = GenerateColor(player_id, player_number);
    Uint32 old_color_center = SDL_MapRGB(image->format, 255, 0, 0); // couleur à remplacer
    Uint32 new_color_center = SDL_MapRGB(image->format, color.r, color.g, color.b); // couleur de remplacement
    Uint32 old_color_border = SDL_MapRGB(image->format, 127, 0, 0); // couleur à remplacer
    Uint32 new_color_border = SDL_MapRGB(image->format, color.r/2, color.g/2, color.b/2); // couleur de remplacement

    Uint32* pixels = (Uint32*)image->pixels;
    int pixelCount = (image->w * image->h);

    for (int i = 0; i < pixelCount; i++)
    {
        if (pixels[i] == old_color_center)
        {
            pixels[i] = new_color_center;
        }
        else if (pixels[i] == old_color_border)
        {
            pixels[i] = new_color_border;
        }
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    return texture;
}
