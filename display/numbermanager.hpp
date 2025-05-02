#ifndef NUMBERMANAGER_HPP
#define NUMBERMANAGER_HPP
#include "display.hpp"
#include "texturemanager.hpp"
#include "constants.hpp"
#include "../model/game.hpp"

class NumberManager
{
    public:
        static void DrawOneNumber(SDL_Renderer* renderer, std::map<int, std::map<int, SDL_Texture*>> textures, SDL_Rect *dest, int to_draw);
        static void DrawNumbers(SDL_Renderer* renderer, std::map<int, std::map<int, SDL_Texture*>> textures, SDL_Rect *dest, int to_draw);
        static void DrawMoneyRecap(SDL_Renderer* renderer, std::map<int, std::map<int, SDL_Texture*>> textures, SDL_Rect *dest, int money, int gain);
};

#endif