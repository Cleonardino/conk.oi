#include "numbermanager.hpp"
#define MAX_NUMBER_SEEN 3

void NumberManager::DrawOneNumber(SDL_Renderer* renderer, std::map<int, std::map<int, SDL_Texture*>> textures, SDL_Rect *dest, int to_draw)
{
    SDL_Rect srcRect;
    srcRect.x = 0 + 6 * (to_draw%10);
    srcRect.y = 0;
    srcRect.w = 6;         // Largeur de la sous-image
    srcRect.h = 9;
    SDL_RenderCopy(renderer, textures[NUMBERS][0], &srcRect, dest);
}

void NumberManager::DrawNumbers(SDL_Renderer* renderer, std::map<int, std::map<int, SDL_Texture*>> textures, SDL_Rect *dest, int to_draw)
{
    SDL_Rect one_number_dest;
    one_number_dest.w = 6 * BUTTON_ZOOM;
    one_number_dest.h = 9 * BUTTON_ZOOM;
    one_number_dest.x = dest->x + one_number_dest.w * (MAX_NUMBER_SEEN - 1);
    one_number_dest.y = dest->y;
    int actu_to_print = to_draw;
    for (int i = 0; i < MAX_NUMBER_SEEN; i++)
    {
        DrawOneNumber(renderer, textures, &one_number_dest, actu_to_print);
        actu_to_print = actu_to_print/10;
        one_number_dest.x = one_number_dest.x - one_number_dest.w;
    }
}

void NumberManager::DrawMoneyRecap(SDL_Renderer* renderer, std::map<int, std::map<int, SDL_Texture*>> textures, SDL_Rect *dest, int money, int gain)
{
    SDL_Rect dest_money, dest_gain;
    dest_money.x = dest->x + (32-18)/2 * BUTTON_ZOOM;
    dest_money.y = dest->y + (32-9*2)/3 * BUTTON_ZOOM;
    dest_gain.x = dest_money.x;
    dest_gain.y = dest_money.y + 9 * BUTTON_ZOOM + (32-9*2)/3 * BUTTON_ZOOM;
    DrawNumbers(renderer, textures, &dest_money, money);
    DrawNumbers(renderer, textures, &dest_gain, gain);
}