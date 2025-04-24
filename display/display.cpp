#include "display.hpp"
#include "texturemanager.hpp"

Display::Display(){}
Display::~Display(){}


SDL_Texture *TileText;
SDL_Rect src, dest;

void Display::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if(fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsystems Initialised" << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window)
        {
            std::cout << "Window created" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer)
        {
            SDL_SetRenderDrawColor(renderer, 50, 125, 160, 255);
            std::cout << "Renderer created" << std::endl;
        }
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
    TileText = TextureManager::LoadTexture("../art/tiles/base_tile.png", renderer);
}

void Display::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
    }
}

void Display::update()
{
    cmt++;
}
void Display::render()
{
    SDL_RenderClear(renderer);
    this->DrawMap();
    SDL_RenderPresent(renderer);
}

void Display::DrawMap()
{
    src.y = src.x = 0;
    dest.x = dest.y = 0;
    src.w = dest.w = 32;
    src.h = dest.h = 32;
    for(int i = 0; i<20;i++)
    {
        for(int j = 0; j<20;j++)
        {
            dest.x = j * 32;
            dest.y = i * 19;
            if ( i%2 == 1)
            {
                dest.x += 16;
            }
            TextureManager::Draw(renderer, TileText, src, dest);
        }
    }
}

void Display::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Display Killed" << std::endl;
}