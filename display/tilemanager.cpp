#include "tilemanager.hpp"

void TileManager::DrawTile(SDL_Renderer* renderer, std::map<int, std::map<int, SDL_Texture*>> textures, SDL_Rect src, SDL_Rect dest, TileDisplayInfos to_draw, const int player_id)
{
    switch(to_draw.get_Tile().get_type())
    {
        case Ocean:
            TextureManager::Draw(renderer, textures[OCEAN_TILE][0], src, dest);
            break;
        case Land:
            TextureManager::Draw(renderer, textures[PLAYERS_TILES][player_id], src, dest);
            break;
        case Forest:
            TextureManager::Draw(renderer, textures[FOREST_TILE][0], src, dest);
            break;
    }
    if (to_draw.get_selected())
    {
        TextureManager::Draw(renderer, textures[SELECTED_TILE][0], src, dest);
    }
    else if(to_draw.get_province_selected())
    {
        TextureManager::Draw(renderer, textures[PROVINCE_SELECTED][0], src, dest);
    }
    for(int i = 0; i < 2; i++)
    {
        if(to_draw.get_walls()[i])
        {
            TextureManager::Draw(renderer, textures[BARRIER_TILE_TL + i][0], src, dest);
        }
    }
    switch (to_draw.get_Tile().get_building().get_type())
    {
        case Wild:
            break;
        case Town:
            TextureManager::Draw(renderer, textures[TOWN_TILE][player_id], src, dest);
            break;
        case Fortress:
            TextureManager::Draw(renderer, textures[FORTRESS_TILE][player_id], src, dest);
            break;
    }
    switch (to_draw.get_Tile().get_character().get_type())
    {
        case Empty:
            break;
        case Peasant:
            TextureManager::Draw(renderer, textures[PEASANT_TILE][player_id], src, dest);
            break;
        case Bandit:
            TextureManager::Draw(renderer, textures[BANDIT_TILE][0], src, dest);
            break;
        case Soldier:
            TextureManager::Draw(renderer, textures[SOLDIER_TILE][player_id], src, dest);
            break;
        case Knight:
            TextureManager::Draw(renderer, textures[KNIGHT_TILE][player_id], src, dest);
            break;
        case Hero:
            TextureManager::Draw(renderer, textures[HERO_TILE][player_id], src, dest);
            break;
    }
    for(int i = 2; i < 6; i++)
    {
        if(to_draw.get_walls()[i])
        {
            TextureManager::Draw(renderer, textures[BARRIER_TILE_TL + i][0], src, dest);
        }
    }
}

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

SDL_Texture* TileManager::LoadTileforPlayer(const int player_id, const int player_number, SDL_Renderer* renderer, const char* filename)
{
    SDL_Surface* image = IMG_Load(filename);
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
