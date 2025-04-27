#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <map>
#include "../model/game.hpp"

class Display {
    private:
        int cmt = 0;
        bool isRunning;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Rect src, dest;
        std::map<int, std::map<int, SDL_Texture*>> textures;
        Game game;
    public:
    Display(Game game_);
    ~Display();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();
    void DrawMap();
    void DrawButton();
    bool InButton(int posx, int posy, int *button_id);
    bool InTile(SDL_Rect dest, int posx_mouse, int posy_mouse);
    bool InMap(int posx, int posy, int* mat_row, int* mat_col);
    bool running() { return isRunning; }

};

#endif

