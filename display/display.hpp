#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include "../model/game.hpp"
#define BUTTON_ZOOM 3

class Display {
    private:
        int cmt = 0;
        int actu_hexa_size;
        bool isRunning;
        bool nowPlaying;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Rect dest;
        coordinates window_center = coordinates(0,0);
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
    void DrawPlay();
    void DrawProvincePanel();
    void DrawPanelButton(SDL_Texture* to_render, int price, int tower_y_displacement);
    bool InProvincePanel(int posx, int posy, int *tile_on);
    bool InPlay(int posx, int posy);
    bool InButton(int posx, int posy, int *button_id);
    bool InTile(SDL_Rect dest, int posx_mouse, int posy_mouse);
    bool InMap(int posx, int posy, int* mat_row, int* mat_col);
    bool running() { return isRunning; }

};

#endif

