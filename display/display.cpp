#include "display.hpp"
#include "texturemanager.hpp"

Display::Display(){}
Display::~Display(){}

#define HEXA_SIZE 32
#define BUTTON_X_SIZE 64 * 2
#define BUTTON_Y_SIZE 32 * 2
#define BUTTON_SPACE 256
#define BASE_TILE 0
#define END_TURN_SIGN 1
#define REWIND_SIGN 2

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
    textures[BASE_TILE] = TextureManager::LoadTexture("../art/tiles/base_tile.png", renderer);
    textures[END_TURN_SIGN] = TextureManager::LoadTexture("../art/tiles/end_turn_sign.png", renderer);
    textures[REWIND_SIGN] = TextureManager::LoadTexture("../art/tiles/rewind_sign.png", renderer);
}

void Display::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x;
                int y;
                Uint32 bitmark;
                bitmark = SDL_GetMouseState(&x,&y);
                if (SDL_BUTTON(bitmark) == 1)
                {
                    std::cout << "x: " << x << "y: " << y << std::endl;
                }
        }
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
    this->DrawButton();
    SDL_RenderPresent(renderer);
}

void Display::DrawMap()
{
    int map_row_size = 20;
    int map_col_size = 20;
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    int map_w = map_row_size * HEXA_SIZE + HEXA_SIZE/2;
    int map_h = (map_col_size+1) * HEXA_SIZE * 19/32;
    src.y = src.x = 0;
    dest.y = 0;
    if (window_h > map_h)
    {
        dest.y = (window_h - map_h) / 2;
    }
    src.w = dest.w = HEXA_SIZE;
    src.h = dest.h = HEXA_SIZE;
    for(int i = 0; i<map_col_size;i++)
    {
        if (window_w > map_w)
        {
            dest.x = (window_w - map_w) / 2 - HEXA_SIZE;
        }
        else
        {
            dest.x = -HEXA_SIZE;
        }
        dest.x += i%2 * HEXA_SIZE / 2;
        for(int j = 0; j<map_row_size;j++)
        {
            dest.x += HEXA_SIZE;
            TextureManager::Draw(renderer, textures[BASE_TILE], src, dest);
        }
        dest.y += HEXA_SIZE * 19/32;
    }
}

bool Display::InMap(int posx_mouse, int posy_mouse, int *mat_row, int *mat_col)
{
    int map_row_size = 20;
    int map_col_size = 20;
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    int map_w = map_row_size * HEXA_SIZE + HEXA_SIZE/2;
    int map_h = (map_col_size+1) * HEXA_SIZE * 19/32;
    src.y = src.x = 0;
    dest.y = 0;
    if (window_h > map_h)
    {
        dest.y = (window_h - map_h) / 2;
    }
    src.w = dest.w = HEXA_SIZE;
    src.h = dest.h = HEXA_SIZE;
    for(int i = 0; i<map_col_size;i++)
    {
        if (window_w > map_w)
        {
            dest.x = (window_w - map_w) / 2 - HEXA_SIZE;
        }
        else
        {
            dest.x = -HEXA_SIZE;
        }
        dest.x += i%2 * HEXA_SIZE / 2;
        for(int j = 0; j<map_row_size;j++)
        {
            dest.x += HEXA_SIZE;
            dest.x = dest.x - posx_mouse;
            dest.y = dest.y - posy_mouse;
            if (dest.x >= 0 and dest.x <= 31 and dest.y >= 0 and dest.y <= 31)
            {
                if (InTile(dest))
                {
                    return 
                }
            }
        }
        dest.y += HEXA_SIZE * 19/32;
    }
}

void Display::DrawButton()
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    src.x = src.y = 0;
    src.w = dest.w = BUTTON_X_SIZE;
    src.h = dest.h = BUTTON_Y_SIZE;
    dest.x = window_w/2 - BUTTON_SPACE/2 - BUTTON_X_SIZE;
    dest.y = window_h - BUTTON_Y_SIZE;
    TextureManager::Draw(renderer, textures[REWIND_SIGN], src, dest);
    dest.x += BUTTON_SPACE + BUTTON_X_SIZE;
    TextureManager::Draw(renderer, textures[END_TURN_SIGN], src, dest);
}

bool Display::InTile(SDL_Rect dest)
{
    if (dest.y >= 13 and dest.y <= 23)
    {
        return true;
    }
    if ((dest.x+1)/2 + dest.y >= 13 and dest.x <= 15 and dest.y <= 13)
    {
        return true;
    }
    if ((-dest.x - 1)/2 + dest.y >= -3 and dest.x >= 16 and dest.y <= 13)
    {
        return true;
    }
    if ((dest.x+1)/2 + dest.y <= 31 + 16  and dest.x >= 16 and dest.y >= 23)
    {
        return true;
    }
    if ((-dest.x - 1)/2 + dest.y <= -8 + 31 and dest.x <= 15 and dest.y >= 23)
    {
        return true;
    }
    return false;
}

void Display::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Display Killed" << std::endl;
}