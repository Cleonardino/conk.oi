#include "texturemanager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* filename, SDL_Renderer* renderer)
{
    SDL_Surface* tmpSurface = IMG_Load(filename);
    if (!tmpSurface) {
        std::cout << "Erreur IMG_Load " << filename << "(" << IMG_GetError() << ")";
        return NULL;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    if (!tex) {
        std::cout << "Erreur SDL_CreateTextureFromSurface :" << SDL_GetError() << std::endl;
    }
    return tex;
}

void TextureManager::Draw(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect *dest)
{
    SDL_RenderCopy(renderer, tex, NULL, dest);
}