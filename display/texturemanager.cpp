#include "texturemanager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* filename, SDL_Renderer* renderer)
{
    SDL_Surface* tmpSurface = IMG_Load(filename);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return tex;
}

void TextureManager::Draw(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(renderer, tex, &src, &dest);
}