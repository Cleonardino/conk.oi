#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <memory>
#include "../model/game.hpp"
#define BUTTON_ZOOM 3

struct SDLTextureDeleter {
    void operator()(SDL_Texture* texture) const {
        if (texture) SDL_DestroyTexture(texture);
    }
};

using UniqueWindow = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>;
using UniqueRenderer = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>>;
using TexturePtr = std::shared_ptr<SDL_Texture>;

class Display {
    public:
    Display(Game game_, Map map);
    ~Display();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void initMap();
    void handleEvents();
    void update();
    void render();
    void clean();
    void DrawMap();
    void DrawButton();
    void DrawPlay();
    void DrawLevel();
    void DrawProvincePanel();
    void DrawPanelButton(SDL_Texture* to_render, int price, int tower_y_displacement);
    void DrawPlayerIndicator();
    void DrawLevelSelector();
    void DrawUnderCursor(TileDisplayInfos to_draw);
    void DrawReturnSign();
    void DrawBackground(int to_draw);
    void DrawVictory();
    bool InReturnSign(int posx, int posy);
    bool InLevelSelector(int posx, int posy, int *level_selected);
    bool InLevel(int posx, int posy);
    bool InProvincePanel(int posx, int posy, int *tile_on);
    bool InPlay(int posx, int posy);
    bool InButton(int posx, int posy, int *button_id);
    bool InTile(SDL_Rect dest, int posx_mouse, int posy_mouse);
    bool InMap(int posx, int posy, int* mat_row, int* mat_col);
    bool running() { return isRunning; }
    private:
        int cmt = 0;
        int actu_hexa_size;
        bool isRunning;
        bool inLevelSelection;
        bool nowPlaying;
        UniqueWindow window;
        UniqueRenderer renderer;
        SDL_Rect dest;
        coordinates window_center = coordinates(0,0);
        std::map<int, std::map<int, TexturePtr>> textures;
        Map map;
        Game game;
};

#endif

