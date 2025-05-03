#include "display.hpp"
#include "texturemanager.hpp"
#include "tilemanager.hpp"
#include "constants.hpp"
#include "numbermanager.hpp"

Display::Display(Game game_): game(game_){}
Display::~Display(){}

#define HEXA_SIZE 32 * 2
#define BUTTON_X_SIZE 64 * BUTTON_ZOOM
#define BUTTON_Y_SIZE 32 * BUTTON_ZOOM
#define PLAY_X_SIZE 64
#define PLAY_Y_SIZE 32
#define BUTTON_SPACE 256 * BUTTON_ZOOM
#define PROVINCE_PANEL_SIZE 192 * BUTTON_ZOOM

void Display::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    nowPlaying = false;
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
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if(renderer)
        {
            SDL_SetRenderDrawColor(renderer, 50, 125, 160, 255);
            std::cout << "Renderer created" << std::endl;
        }
        isRunning = true;
        int map_row_size = game.get_width();
        int map_col_size = game.get_height();
        int window_w;
        int window_h;
        SDL_GetWindowSize(window, &window_w, &window_h);
        window_h = window_h - BUTTON_Y_SIZE;
        actu_hexa_size = std::min(window_w/(map_row_size+1), window_h/map_col_size * 32/19);
        std::cout << actu_hexa_size << std::endl;
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
    int nb_players = game.get_max_player_count();
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
    textures[PLAY_BUTTON][0] = TextureManager::LoadTexture("../art/buttons/play.png", renderer);
    textures[VALID_DESTINATION][0] = TextureManager::LoadTexture("../art/tiles/valid_dest.png", renderer);
    textures[SLEEPING_CHAR][0] = TextureManager::LoadTexture("../art/characters/sleeping.png", renderer);
    textures[PROVINCE_PANEL][0] = TextureManager::LoadTexture("../art/buttons/province_panel.png", renderer);
    textures[NUMBERS][0] = TextureManager::LoadTexture("../art/font/numbers.png", renderer);
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
                                game.on_rewind();
                                break;
                            case END_TURN_SIGN:
                                game.on_end_turn();
                                break;
                        }
                    }
                    else if (InPlay(x, y))
                    {
                        nowPlaying = true;
                    }
                    else if(InMap(x,y,&mat_i,&mat_j))
                    {
                        game.on_tile_click(coordinates(mat_i, mat_j));
                    }
                }
                break;
            case SDL_MOUSEWHEEL:
                if (actu_hexa_size > 0 || (actu_hexa_size == 0 and event.wheel.y > 0))
                {
                    actu_hexa_size += event.wheel.y * 8;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT:  window_center.first += 10; break;
                    case SDLK_RIGHT: window_center.first -= 10; break;
                    case SDLK_UP:    window_center.second += 10; break;
                    case SDLK_DOWN:  window_center.second -= 10; break;
                    case SDLK_ESCAPE: SDL_SetWindowFullscreen(window, 0); break;
                }
                break;
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
    if (nowPlaying)
    {
        this->DrawMap();
        this->DrawButton();
    }
    else
    {
        this->DrawPlay();
    }
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
    int map_w = map_row_size * actu_hexa_size + actu_hexa_size/2;
    int map_h = map_col_size * actu_hexa_size * 19/32;
    dest.y = 0;
    if (window_h > map_h)
    {
        dest.y = (window_h - map_h) / 2;
    }
    dest.w = actu_hexa_size;
    dest.h = actu_hexa_size;
    dest.y += window_center.second;
    for(int i = 0; i<map_col_size;i++)
    {
        if (window_w > map_w)
        {
            dest.x = (window_w - map_w) / 2 - actu_hexa_size;
        }
        else
        {
            dest.x = -actu_hexa_size;
        }
        dest.x += i%2 * actu_hexa_size / 2 + window_center.first;
        for(int j = 0; j<map_row_size;j++)
        {
            dest.x += actu_hexa_size;
            TileManager::DrawTile(renderer, textures, &dest, game.get_display_infos(coordinates(i,j)), game.get_display_infos(coordinates(i,j)).get_Tile().get_owner() + 2);
        }
        dest.y += actu_hexa_size * 19/32;
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
    int map_w = map_row_size * actu_hexa_size + actu_hexa_size/2;
    int map_h = map_col_size * actu_hexa_size * 19/32;
    SDL_Rect actuR;
    dest.y = 0;
    if (window_h > map_h)
    {
        dest.y = (window_h - map_h) / 2;
    }
    dest.w = actuR.w = actu_hexa_size;
    dest.h = actuR.h = actu_hexa_size;
    dest.y += window_center.second;
    for(int i = 0; i<map_col_size;i++)
    {
        if (window_w > map_w)
        {
            dest.x = (window_w - map_w) / 2 - actu_hexa_size;
        }
        else
        {
            dest.x = -actu_hexa_size;
        }
        dest.x += i%2 * actu_hexa_size / 2 + window_center.first;
        for(int j = 0; j<map_row_size;j++)
        {
            dest.x += actu_hexa_size;
            actuR.x = posx_mouse - dest.x;
            actuR.y = posy_mouse - dest.y;
            if (actuR.x >= 0 and actuR.x <= actu_hexa_size - 1 and actuR.y >= 0 and actuR.y <= actu_hexa_size - 1)
            {
                if (InTile(dest, posx_mouse, posy_mouse))
                {
                    *mat_row = i;
                    *mat_col = j;
                    return true;
                }
            }
        }
        dest.y += actu_hexa_size * 19/32;
    }
    return false;
}

bool Display::InTile(SDL_Rect dest, int posx_mouse, int posy_mouse)
{
    int mouse_x_in_tile = (posx_mouse - dest.x) * 32/actu_hexa_size; // X Mouse position in tile dest referential
    int mouse_y_in_tile = (posy_mouse - dest.y) * 32/actu_hexa_size; // Y Mouse position in tile dest referential

    if (mouse_y_in_tile >= 13 and mouse_y_in_tile <= (24 - 1)) // Verification for center of the tile (base_tile dependant)
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
    dest.w = BUTTON_X_SIZE;
    dest.h = BUTTON_Y_SIZE;
    dest.x = window_w/2 - BUTTON_SPACE/2 - BUTTON_X_SIZE;
    dest.y = window_h - BUTTON_Y_SIZE;
    TextureManager::Draw(renderer, textures[REWIND_SIGN][0], &dest);
    DrawProvincePanel();
    dest.w = BUTTON_X_SIZE;
    dest.x += 32 * BUTTON_ZOOM + (BUTTON_SPACE - PROVINCE_PANEL_SIZE)/2;
    TextureManager::Draw(renderer, textures[END_TURN_SIGN][0], &dest);
}

void Display::DrawProvincePanel()
{
    int player_turn_id = 0; // Placeholder
    dest.x += BUTTON_X_SIZE + (BUTTON_SPACE - PROVINCE_PANEL_SIZE)/2;
    dest.w = PROVINCE_PANEL_SIZE;
    TextureManager::Draw(renderer, textures[PROVINCE_PANEL][0], &dest);
    dest.w = 32 * BUTTON_ZOOM;
    DrawPanelButton(textures[PEASANT_TILE][player_turn_id], 10, 0);//Placeholders for price
    dest.x += 32 * BUTTON_ZOOM;
    DrawPanelButton(textures[SOLDIER_TILE][player_turn_id], 9, 0);//Placeholders for price
    dest.x += 32 * BUTTON_ZOOM;
    DrawPanelButton(textures[KNIGHT_TILE][player_turn_id], 8, 0);//Placeholders for price
    dest.x += 32 * BUTTON_ZOOM;
    DrawPanelButton(textures[HERO_TILE][player_turn_id], 7, 0);//Placeholders for price
    dest.x += 32 * BUTTON_ZOOM;
    DrawPanelButton(textures[FORTRESS_TILE][player_turn_id], 6, 4 * BUTTON_ZOOM);//Placeholders for price
    dest.x += 36 * BUTTON_ZOOM;
    NumberManager::DrawMoneyRecap(renderer, textures, &dest, 2, 9); //Placeholders for price and gain
    dest.x -= 4 * BUTTON_ZOOM;
}

void Display::DrawPanelButton(SDL_Texture* to_render, int price, int tower_y_displacement)
{
    dest.y += tower_y_displacement;
    dest.y -= 5 * BUTTON_ZOOM;
    TextureManager::Draw(renderer, to_render, &dest);
    dest.y += 27 * BUTTON_ZOOM;
    dest.x += (32 - 18) / 2 * BUTTON_ZOOM;
    dest.y -= tower_y_displacement;
    NumberManager::DrawNumbers(renderer, textures, &dest, price);
    dest.x -= (32 - 18) / 2 * BUTTON_ZOOM;
    dest.y -= 22 * BUTTON_ZOOM;
}

bool Display::InButton(int posx, int posy, int *button_id)
{
    int window_w;
    int window_h;
    SDL_Rect actuR;
    SDL_GetWindowSize(window, &window_w, &window_h);
    dest.w = actuR.w = BUTTON_X_SIZE;
    dest.h = actuR.h = BUTTON_Y_SIZE;
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

void Display::DrawPlay()
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    dest.w = PLAY_X_SIZE * 5;
    dest.h = PLAY_Y_SIZE * 5;
    dest.x = window_w/2 - dest.w/2;
    dest.y = window_h/2 - dest.h/2;
    TextureManager::Draw(renderer, textures[PLAY_BUTTON][0], &dest);
}

bool Display::InPlay(int posx, int posy)
{
    if (nowPlaying)
    {
        return false;
    }
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    dest.w = PLAY_X_SIZE * 5;
    dest.h = PLAY_Y_SIZE * 5;
    dest.x = window_w/2 - dest.w/2;
    dest.y = window_h/2 - dest.h/2;
    if (posx - dest.x >= 0 and posx - dest.x <= dest.w and posy - dest.y >= 0 and posy - dest.y <= dest.h)
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