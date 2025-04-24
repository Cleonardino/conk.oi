#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

class Display {
    private:
        int cmt = 0;
        bool isRunning;
        SDL_Window *window;
        SDL_Renderer *renderer;
        int map[20][20];
    public:
    Display();
    ~Display();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();
    void DrawMap();
    bool running() { return isRunning; }

};

#endif

