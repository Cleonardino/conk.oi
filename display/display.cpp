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
    inLevelSelection = false;
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
    for (int i = -1; i < nb_players; i++)
    {
        textures[FORTRESS_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/tiles/fortress.png");
        textures[TOWN_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/tiles/town.png");
        textures[PEASANT_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/characters/peasant.png");
        textures[HERO_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/characters/hero.png");
        textures[KNIGHT_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/characters/knight.png");
        textures[SOLDIER_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer,"../art/characters/soldier.png");
        textures[PLAYERS_TILES][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer, "../art/tiles/land.png");
        textures[PLAYER_TURN_INDICATION][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer, "../art/buttons/player_turn_indication.png");
    }
    textures[END_TURN_SIGN][0] = TextureManager::LoadTexture("../art/buttons/end_turn_sign.png", renderer);
    textures[REWIND_SIGN][0] = TextureManager::LoadTexture("../art/buttons/rewind_sign.png", renderer);
    textures[PLAY_BUTTON][0] = TextureManager::LoadTexture("../art/buttons/play.png", renderer);
    textures[LEVEL_BUTTON][0] = TextureManager::LoadTexture("../art/buttons/levels_sign.png", renderer);
    textures[VALID_DESTINATION][0] = TextureManager::LoadTexture("../art/tiles/valid_dest.png", renderer);
    textures[SLEEPING_CHAR][0] = TextureManager::LoadTexture("../art/characters/sleeping.png", renderer);
    textures[PROVINCE_PANEL][0] = TextureManager::LoadTexture("../art/buttons/province_panel.png", renderer);
    textures[NUMBERS][0] = TextureManager::LoadTexture("../art/font/numbers.png", renderer);
    textures[PANEL_SELECTED][0] = TextureManager::LoadTexture("../art/buttons/panel_selected.png", renderer);
    textures[ONE_LEVEL_BACKGROUND][0] = TextureManager::LoadTexture("../art/buttons/one_level_background.png", renderer);
    textures[RETURN_SIGN][0] = TextureManager::LoadTexture("../art/buttons/return.png", renderer);
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
                int tile_on;
                Uint32 bitmark;
                bitmark = SDL_GetMouseState(&x,&y);
                if (SDL_BUTTON(bitmark) == 1)
                {
                    if(InProvincePanel(x, y, &tile_on))
                    {
                        switch(tile_on)
                        {
                            case PEASANT_TILE:
                                game.on_peasant_purchase();
                                break;
                            case SOLDIER_TILE:
                                game.on_soldier_purchase();
                                break;
                            case KNIGHT_TILE:
                                game.on_knight_purchase();
                                break;
                            case HERO_TILE:
                                game.on_hero_purchase();
                                break;
                            case FORTRESS_TILE:
                                game.on_fortress_purchase();
                                break;
                        }
                    }
                    else if(InButton(x,y,&button_id))
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
                    else if (InLevel(x, y))
                    {
                        inLevelSelection = true;
                        std::cout << "Levels" << std::endl;
                    }
                    else if(InMap(x,y,&mat_i,&mat_j))
                    {
                        game.on_tile_click(coordinates(mat_i, mat_j));
                    }
                }
                break;
            case SDL_MOUSEWHEEL:
                if (actu_hexa_size > 0 || (actu_hexa_size <= 0 and event.wheel.y > 0))
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
        this->DrawPlayerIndicator();
        this->DrawUnderCursor(game.get_cursor_infos());
    }
    else if (inLevelSelection)
    {
        this->DrawLevelSelector();
    }
    else
    {
        this->DrawPlay();
        this->DrawLevel();
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
            TileManager::DrawTile(renderer, textures, &dest, game.get_display_infos(coordinates(i,j)), game.get_display_infos(coordinates(i,j)).get_Tile().get_owner());
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
    dest.x += BUTTON_X_SIZE + (BUTTON_SPACE - PROVINCE_PANEL_SIZE)/2;
    DrawProvincePanel();
    dest.w = BUTTON_X_SIZE;
    dest.x += PROVINCE_PANEL_SIZE + (BUTTON_SPACE - PROVINCE_PANEL_SIZE)/2;
    TextureManager::Draw(renderer, textures[END_TURN_SIGN][0], &dest);
}

void Display::DrawProvincePanel()
{
    int player_turn_id = game.get_active_player_id();
    int to_restore = dest.x;
    int gold = game.get_displayed_gold();
    dest.w = PROVINCE_PANEL_SIZE;
    TextureManager::Draw(renderer, textures[PROVINCE_PANEL][0], &dest);
    dest.w = 32 * BUTTON_ZOOM;
    if (gold >= PEASANT_COST)
        DrawPanelButton(textures[PEASANT_TILE][player_turn_id], PEASANT_COST, 0);
    dest.x += 32 * BUTTON_ZOOM;
    if (gold >= SOLDIER_COST)
        DrawPanelButton(textures[SOLDIER_TILE][player_turn_id], SOLDIER_COST, 0);
    dest.x += 32 * BUTTON_ZOOM;
    if (gold >= KNIGHT_COST)
        DrawPanelButton(textures[KNIGHT_TILE][player_turn_id], KNIGHT_COST, 0);
    dest.x += 32 * BUTTON_ZOOM;
    if (gold >= HERO_COST)
        DrawPanelButton(textures[HERO_TILE][player_turn_id], HERO_COST, 0);
    dest.x += 32 * BUTTON_ZOOM;
    if (gold >= FORTRESS_COST)
        DrawPanelButton(textures[FORTRESS_TILE][player_turn_id], FORTRESS_COST, 4 * BUTTON_ZOOM);
    dest.x += 36 * BUTTON_ZOOM;
    NumberManager::DrawMoneyRecap(renderer, textures, &dest, game.get_displayed_gold(), game.get_displayed_income());
    dest.x = to_restore;
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

void Display::DrawPlayerIndicator()
{
    int actu_player_turn = game.get_active_player_id();
    int next_player_turn = game.get_next_player_id();
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    dest.w = 32 * BUTTON_ZOOM;
    dest.h = 32 * BUTTON_ZOOM;
    dest.x = window_w - 32 * BUTTON_ZOOM - 32;
    dest.y = 10 * BUTTON_ZOOM;
    TextureManager::Draw(renderer, textures[PLAYER_TURN_INDICATION][actu_player_turn], &dest);
    dest.w = dest.w/2;
    dest.h = dest.h/2;
    dest.x += 20 * BUTTON_ZOOM;
    dest.y -= 4 * BUTTON_ZOOM;
    TextureManager::Draw(renderer, textures[PLAYER_TURN_INDICATION][next_player_turn], &dest);
}

void Display::DrawUnderCursor(TileDisplayInfos to_draw)
{
    int x;
    int y;
    Uint32 bitmark;
    bitmark = SDL_GetMouseState(&x,&y);
    dest.w = 32 * BUTTON_ZOOM;
    dest.h = 32 * BUTTON_ZOOM;
    dest.x = x - dest.w/2;
    dest.y = y - dest.h/2;
    if (to_draw.get_Tile().get_building().get_type() == Fortress)
    {
        TextureManager::Draw(renderer, textures[FORTRESS_TILE][game.get_active_player_id()], &dest);
    }
    switch (to_draw.get_Tile().get_character().get_type())
    {
        case Peasant:
            TextureManager::Draw(renderer, textures[PEASANT_TILE][game.get_active_player_id()], &dest);
            break;
        case Soldier:
            TextureManager::Draw(renderer, textures[SOLDIER_TILE][game.get_active_player_id()], &dest);
            break;
        case Knight:
            TextureManager::Draw(renderer, textures[KNIGHT_TILE][game.get_active_player_id()], &dest);
            break;
        case Hero:
            TextureManager::Draw(renderer, textures[HERO_TILE][game.get_active_player_id()], &dest);
            break;
    }
}

bool Display::InProvincePanel(int posx, int posy, int *tile_on)
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    dest.w = 32 * BUTTON_ZOOM;
    dest.h = 32 * BUTTON_ZOOM;
    dest.x = window_w/2 - BUTTON_SPACE/2 + (BUTTON_SPACE - PROVINCE_PANEL_SIZE)/2;
    dest.y = window_h - BUTTON_Y_SIZE;
    int posx_mouse = posx - dest.x;
    int posy_mouse = posy - dest.y;
    int in_panel = -1;
    int gold = game.get_displayed_gold();
    for (int i = 0; i < 5;i++)
    {
        if (posx_mouse >= 0 + dest.w * i and posx_mouse <= dest.w * (i+1) and posy_mouse >= 0)
        {
            in_panel = i;
        }
    }
    dest.x += dest.w * in_panel;
    switch (in_panel)
    {
    case 0:
        *tile_on = PEASANT_TILE;
        return gold >= PEASANT_COST;
    case 1:
        *tile_on = SOLDIER_TILE;
        return gold >= SOLDIER_COST;
    case 2:
        *tile_on = KNIGHT_TILE;
        return gold >= KNIGHT_COST;
    case 3:
        *tile_on = HERO_TILE;
        return gold >= HERO_COST;
    case 4:
        *tile_on = FORTRESS_TILE;
        return gold >= FORTRESS_COST;
    default:
        return false;
    }
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

void Display::DrawLevel()
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    dest.w = PLAY_X_SIZE * 2;
    dest.h = PLAY_Y_SIZE * 2;
    dest.x = window_w/2 - dest.w/2;
    dest.y = window_h/2 - dest.h/2 + PLAY_Y_SIZE * 5;
    TextureManager::Draw(renderer, textures[LEVEL_BUTTON][0], &dest);
}

void Display::DrawLevelSelector()
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    dest.w = 32 * BUTTON_ZOOM;
    dest.h = 32 * BUTTON_ZOOM;
    dest.x = (window_w - 5 * dest.w)/6;
    dest.y = dest.h;
    for (int i = 1; i < 9; i++)
    {
        TextureManager::Draw(renderer, textures[ONE_LEVEL_BACKGROUND][0], &dest);
        NumberManager::DrawOneNumber(renderer, textures, &dest, i);
        dest.x += dest.w + (window_w - 5 * dest.w)/6;
        if (i%5 == 0)
        {
            dest.x = (window_w - 5 * dest.w)/6;
            dest.y += dest.h * 2;
        }
    }
}

bool Display::InLevel(int posx, int posy)
{
    if (nowPlaying)
    {
        return false;
    }
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    dest.w = PLAY_X_SIZE * 2;
    dest.h = PLAY_Y_SIZE * 2;
    dest.x = window_w/2 - dest.w/2;
    dest.y = window_h/2 - dest.h/2 + PLAY_Y_SIZE * 5;
    return (posx - dest.x >= 0 and posx - dest.x <= dest.w and posy - dest.y >= 0 and posy - dest.y <= dest.h);
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