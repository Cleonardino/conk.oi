#include "display.hpp"
#include "texturemanager.hpp"
#include "tilemanager.hpp"
#include "constants.hpp"

Display::Display(Game game_): game(game_){}
Display::~Display(){}

#define HEXA_SIZE 32 * 2
#define BUTTON_X_SIZE 64 * 2
#define BUTTON_Y_SIZE 32 * 2
#define BUTTON_SPACE 256

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
    textures[BASE_TILE][0] = TileManager::LoadTile(BASE_TILE, renderer);
    textures[FOREST_TILE][0] = TileManager::LoadTile(FOREST_TILE, renderer);
    textures[OCEAN_TILE][0] = TileManager::LoadTile(OCEAN_TILE, renderer);
    textures[BARRIER_TILE_BL][0] = TextureManager::LoadTexture("../art/tiles/wall_bl.png", renderer);
    textures[BARRIER_TILE_BR][0] = TextureManager::LoadTexture("../art/tiles/wall_br.png", renderer);
    textures[BARRIER_TILE_L][0] = TextureManager::LoadTexture("../art/tiles/wall_l.png", renderer);
    textures[BARRIER_TILE_R][0] = TextureManager::LoadTexture("../art/tiles/wall_r.png", renderer);
    textures[BARRIER_TILE_TL][0] = TextureManager::LoadTexture("../art/tiles/wall_tl.png", renderer);
    textures[BARRIER_TILE_TR][0] = TextureManager::LoadTexture("../art/tiles/wall_tr.png", renderer);
    textures[CAMP_TILE][0] = TextureManager::LoadTexture("../art/tiles/camp.png", renderer);
    textures[PROVINCE_SELECTED][0] = TextureManager::LoadTexture("../art/tiles/province_selected.png", renderer);
    textures[SELECTED_TILE][0] = TextureManager::LoadTexture("../art/tiles/selected.png", renderer);
    textures[BANDIT_TILE][0] = TextureManager::LoadTexture("../art/characters/bandit.png", renderer);
    int nb_players = 5;
    for (int i = 1; i < nb_players; i++)
    {
        textures[FORTRESS_TILE][i-1] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/tiles/fortress.png");
        textures[TOWN_TILE][i-1] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/tiles/town.png");
        textures[PEASANT_TILE][i-1] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/characters/peasant.png");
        textures[HERO_TILE][i-1] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/characters/hero.png");
        textures[KNIGHT_TILE][i-1] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/characters/knight.png");
        textures[SOLDIER_TILE][i-1] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/characters/soldier.png");
        textures[PLAYERS_TILES][i-1] = TileManager::LoadTileforPlayer(i, nb_players, renderer, "../art/tiles/land.png");
    }
    textures[END_TURN_SIGN][0] = TextureManager::LoadTexture("../art/buttons/end_turn_sign.png", renderer);
    textures[REWIND_SIGN][0] = TextureManager::LoadTexture("../art/buttons/rewind_sign.png", renderer);
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
                int mat_i;
                int mat_j;
                int button_id;
                Uint32 bitmark;
                bitmark = SDL_GetMouseState(&x,&y);
                if (SDL_BUTTON(bitmark) == 1)
                {
                    if(InButton(x,y,&button_id))
                    {
                        switch(button_id)
                        {
                            case REWIND_SIGN:
                                std::cout << "Rewind sign clicked" << std::endl;
                                break;
                            case END_TURN_SIGN:
                                std::cout << "End turn sign clicked" << std::endl;
                                break;
                        }
                    }
                    else if(InMap(x,y,&mat_i,&mat_j))
                    {
                        std::cout << "i: " << mat_i << " j: " << mat_j << std::endl;
                    }
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
    int map_row_size = game.get_width();
    int map_col_size = game.get_height();
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    window_h = window_h - BUTTON_Y_SIZE;
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
            TileManager::DrawTile(renderer, textures, src, dest, game.get_display_infos(coordinates(i,j)), game.get_display_infos(coordinates(i,j)).get_Tile().get_owner() + 2);
        }
        dest.y += HEXA_SIZE * 19/32;
    }
}

bool Display::InMap(int posx_mouse, int posy_mouse, int *mat_row, int *mat_col)
{
    int map_row_size = game.get_width();
    int map_col_size = game.get_height();
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    window_h = window_h - BUTTON_Y_SIZE;
    int map_w = map_row_size * HEXA_SIZE + HEXA_SIZE/2;
    int map_h = (map_col_size+1) * HEXA_SIZE * 19/32;
    SDL_Rect actuR;
    src.y = src.x = 0;
    dest.y = 0;
    if (window_h > map_h)
    {
        dest.y = (window_h - map_h) / 2;
    }
    src.w = dest.w = actuR.w = HEXA_SIZE;
    src.h = dest.h = actuR.h =HEXA_SIZE;
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
            actuR.x = posx_mouse - dest.x;
            actuR.y = posy_mouse - dest.y;
            if (actuR.x >= 0 and actuR.x <= HEXA_SIZE - 1 and actuR.y >= 0 and actuR.y <= HEXA_SIZE - 1)
            {
                if (InTile(dest, posx_mouse, posy_mouse))
                {
                    *mat_row = i;
                    *mat_col = j;
                    return true;
                }
            }
        }
        dest.y += HEXA_SIZE * 19/32;
    }
    return false;
}

bool Display::InTile(SDL_Rect dest, int posx_mouse, int posy_mouse)
{
    int mouse_x_in_tile = (posx_mouse - dest.x) * 32/(HEXA_SIZE); // X Mouse position in tile dest referential
    int mouse_y_in_tile = (posy_mouse - dest.y) * 32/(HEXA_SIZE); // Y Mouse position in tile dest referential

    if (mouse_y_in_tile >= 13 and mouse_y_in_tile <= 24 - 1) // Verification for center of the tile (base_tile dependant)
    {
        return true;
    }
    if ((mouse_x_in_tile + 1)/2 + mouse_y_in_tile >= 13 and mouse_x_in_tile < 16 and mouse_y_in_tile <= 13) // Verification for left-hupper corner of the tile (base_tile dependant)
    {
        return true;
    }
    if (-1 * (mouse_x_in_tile + 1)/2 + mouse_y_in_tile >= -3 and mouse_x_in_tile >= 16 and mouse_y_in_tile <= 13)  // Verification for right-hupper corner of the tile (base_tile dependant)
    {
        return true;
    }
    if ((mouse_x_in_tile + 1)/2 + mouse_y_in_tile <= (31 + 8) and mouse_x_in_tile >= 16 and mouse_y_in_tile >= 24 - 1)  // Verification for right-bottom corner of the tile (base_tile dependant)
    {
        return true;
    }
    if (-1 * (mouse_x_in_tile + 1)/2 + mouse_y_in_tile <= (-8 + 31) and mouse_x_in_tile < 16 and mouse_y_in_tile >= 24 - 1)  // Verification for left-bottom corner of the tile (base_tile dependant)
    {
        return true;
    }
    return false;
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
    TextureManager::Draw(renderer, textures[REWIND_SIGN][0], src, dest);
    dest.x += BUTTON_SPACE + BUTTON_X_SIZE;
    TextureManager::Draw(renderer, textures[END_TURN_SIGN][0], src, dest);
}

bool Display::InButton(int posx, int posy, int *button_id)
{
    int window_w;
    int window_h;
    SDL_Rect actuR;
    SDL_GetWindowSize(window, &window_w, &window_h);
    src.x = src.y = 0;
    src.w = dest.w = actuR.w = BUTTON_X_SIZE;
    src.h = dest.h = actuR.h = BUTTON_Y_SIZE;
    dest.x = window_w/2 - BUTTON_SPACE/2 - BUTTON_X_SIZE;
    dest.y = window_h - BUTTON_Y_SIZE;
    actuR.x = posx - dest.x;
    actuR.y = posy - dest.y;
    if(actuR.x >= 0 and actuR.x <= BUTTON_X_SIZE and actuR.y >= 0)
    {
        *button_id = REWIND_SIGN;
        return true;
    }
    dest.x += BUTTON_SPACE + BUTTON_X_SIZE;
    actuR.x = posx - dest.x;
    if(actuR.x >= 0 and actuR.x <= BUTTON_X_SIZE and actuR.y >= 0)
    {
        *button_id = END_TURN_SIGN;
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