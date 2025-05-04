#include "display.hpp"
#include "texturemanager.hpp"
#include "tilemanager.hpp"
#include "constants.hpp"
#include "numbermanager.hpp"

Display::Display(Game game_, Map map_): game(game_), map(map_) {}
Display::~Display(){}

#define HEXA_SIZE 32 * 2
#define BUTTON_X_SIZE 64 * BUTTON_ZOOM
#define BUTTON_Y_SIZE 32 * BUTTON_ZOOM
#define PLAY_X_SIZE 64
#define PLAY_Y_SIZE 32
#define BUTTON_SPACE 256 * BUTTON_ZOOM
#define PROVINCE_PANEL_SIZE 192 * BUTTON_ZOOM
#define LEVEL_NUMBER 4

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

        window = UniqueWindow(SDL_CreateWindow(title, xpos, ypos, width, height, flags), SDL_DestroyWindow);
        if(window.get())
        {
            std::cout << "Window created" << std::endl;
        }
        renderer = UniqueRenderer(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_SOFTWARE), SDL_DestroyRenderer);
        if(renderer)
        {
            SDL_SetRenderDrawColor(renderer.get(), 50, 125, 160, 255);
            std::cout << "Renderer created" << std::endl;
        }
        isRunning = true;
        initMap();
    }
    else
    {
        isRunning = false;
    }
}

void Display::initMap()
{
    int map_row_size = game.get_width();
    int map_col_size = game.get_height();
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    window_h = window_h - BUTTON_Y_SIZE;
    actu_hexa_size = std::min(window_w/(map_row_size+1), window_h/map_col_size * 32/19);
    std::cout << actu_hexa_size << std::endl;
    textures[BASE_TILE][0] = TileManager::LoadTile(BASE_TILE, renderer.get());
    textures[FOREST_TILE][0] = TileManager::LoadTile(FOREST_TILE, renderer.get());
    textures[OCEAN_TILE][0] = TileManager::LoadTile(OCEAN_TILE, renderer.get());
    textures[BARRIER_TILE_BL][0] = TextureManager::LoadTexture("../art/tiles/wall_bl.png", renderer.get());
    textures[BARRIER_TILE_BR][0] = TextureManager::LoadTexture("../art/tiles/wall_br.png", renderer.get());
    textures[BARRIER_TILE_L][0] = TextureManager::LoadTexture("../art/tiles/wall_l.png", renderer.get());
    textures[BARRIER_TILE_R][0] = TextureManager::LoadTexture("../art/tiles/wall_r.png", renderer.get());
    textures[BARRIER_TILE_TL][0] = TextureManager::LoadTexture("../art/tiles/wall_tl.png", renderer.get());
    textures[BARRIER_TILE_TR][0] = TextureManager::LoadTexture("../art/tiles/wall_tr.png", renderer.get());
    textures[CAMP_TILE][0] = TextureManager::LoadTexture("../art/tiles/camp.png", renderer.get());
    textures[PROVINCE_SELECTED][0] = TextureManager::LoadTexture("../art/tiles/province_selected.png", renderer.get());
    textures[SELECTED_TILE][0] = TextureManager::LoadTexture("../art/tiles/selected.png", renderer.get());
    textures[BANDIT_TILE][0] = TextureManager::LoadTexture("../art/characters/bandit.png", renderer.get());
    int nb_players = game.get_max_player_count();
    for (int i = -1; i < nb_players; i++)
    {
        textures[FORTRESS_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer.get(),"../art/tiles/fortress.png");
        textures[TOWN_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer.get(),"../art/tiles/town.png");
        textures[PEASANT_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer.get(),"../art/characters/peasant.png");
        textures[HERO_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer.get(),"../art/characters/hero.png");
        textures[KNIGHT_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer.get(),"../art/characters/knight.png");
        textures[SOLDIER_TILE][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer.get(),"../art/characters/soldier.png");
        textures[PLAYERS_TILES][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer.get(), "../art/tiles/land.png");
        textures[PLAYER_TURN_INDICATION][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer.get(), "../art/buttons/player_turn_indication.png");
        textures[VICTORY_SIGN][i] = TileManager::LoadTileforPlayer(i, nb_players, renderer.get(), "../art/buttons/victory_sign.png");
    }
    textures[END_TURN_SIGN][0] = TextureManager::LoadTexture("../art/buttons/end_turn_sign.png", renderer.get());
    textures[REWIND_SIGN][0] = TextureManager::LoadTexture("../art/buttons/rewind_sign.png", renderer.get());
    textures[PLAY_BUTTON][0] = TextureManager::LoadTexture("../art/buttons/play.png", renderer.get());
    textures[LEVEL_BUTTON][0] = TextureManager::LoadTexture("../art/buttons/levels_sign.png", renderer.get());
    textures[VALID_DESTINATION][0] = TextureManager::LoadTexture("../art/tiles/valid_dest.png", renderer.get());
    textures[SLEEPING_CHAR][0] = TextureManager::LoadTexture("../art/characters/sleeping.png", renderer.get());
    textures[PROVINCE_PANEL][0] = TextureManager::LoadTexture("../art/buttons/province_panel.png", renderer.get());
    textures[NUMBERS][0] = TextureManager::LoadTexture("../art/font/numbers.png", renderer.get());
    textures[PANEL_SELECTED][0] = TextureManager::LoadTexture("../art/buttons/panel_selected.png", renderer.get());
    textures[ONE_LEVEL_BACKGROUND][0] = TextureManager::LoadTexture("../art/buttons/one_level_background.png", renderer.get());
    textures[RETURN_SIGN][0] = TextureManager::LoadTexture("../art/buttons/return.png", renderer.get());
    textures[MAIN_MENU_BACKGROUND][0] = TextureManager::LoadTexture("../art/buttons/mainmenu_background.png", renderer.get());
    textures[LEVEL_SELECTOR_BACKGROUND][0] = TextureManager::LoadTexture("../art/buttons/level_selector_background.png", renderer.get());
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
                if (game.get_finished())
                {
                    break;
                }
                int x;
                int y;
                int mat_i;
                int mat_j;
                int button_id;
                int tile_on;
                int level_seletion;
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
                    else if (InReturnSign(x,y))
                    {
                        nowPlaying = false;
                        inLevelSelection = false;
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
                    }
                    else if (InLevelSelector(x, y, &level_seletion))
                    {
                        switch (level_seletion)
                        {
                        case 1:
                            map = parse_csv("../example_map.txt");
                            game = Game(map,0,std::vector<Province>());
                            inLevelSelection = false;
                            initMap();
                            break;
                        case 2:
                            map = parse_csv("../map_level_2.txt");
                            game = Game(map,0,std::vector<Province>());
                            inLevelSelection = false;
                            initMap();
                            break;
                        case 3:
                            map = parse_csv("../map_level_3.txt");
                            game = Game(map,0,std::vector<Province>());
                            inLevelSelection = false;
                            initMap();
                            break;
                        case 4:
                            map = parse_csv("../map_level_4.txt");
                            game = Game(map,0,std::vector<Province>());
                            inLevelSelection = false;
                            initMap();
                            break;
                        default:
                            break;
                        }
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
                if (game.get_finished())
                {
                    break;
                }
                switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT:  window_center.first += 10; break;
                    case SDLK_RIGHT: window_center.first -= 10; break;
                    case SDLK_UP:    window_center.second += 10; break;
                    case SDLK_DOWN:  window_center.second -= 10; break;
                    case SDLK_ESCAPE: SDL_SetWindowFullscreen(window.get(), 0); break;
                    case SDLK_1:
                    case SDLK_KP_1: 
                    if (game.do_display_panel() and game.get_displayed_gold() >= PEASANT_COST)
                        game.on_peasant_purchase();
                    break;
                    case SDLK_2:
                    case SDLK_KP_2:
                    if (game.do_display_panel() and game.get_displayed_gold() >= SOLDIER_COST)
                        game.on_soldier_purchase();
                    break;
                    case SDLK_3:
                    case SDLK_KP_3:
                    if (game.do_display_panel() and game.get_displayed_gold() >= KNIGHT_COST)
                        game.on_knight_purchase();
                    break;
                    case SDLK_4:
                    case SDLK_KP_4:
                    if (game.do_display_panel() and game.get_displayed_gold() >= HERO_COST)
                        game.on_hero_purchase();
                    break;
                    case SDLK_f:
                    if (game.do_display_panel() and game.get_displayed_gold() >= FORTRESS_COST)
                        game.on_fortress_purchase();
                    break;
                    case SDLK_r:
                    game.on_rewind();
                    break;
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
    SDL_RenderClear(renderer.get());
    if (nowPlaying)
    {
        this->DrawBackground(IN_GAME_BACKGROUND);
        this->DrawMap();
        this->DrawButton();
        this->DrawPlayerIndicator();
        this->DrawReturnSign();
        this->DrawUnderCursor(game.get_cursor_infos());
        if (game.get_finished())
        {
            this->DrawVictory();
        }
    }
    else if (inLevelSelection)
    {
        this->DrawBackground(LEVEL_SELECTOR_BACKGROUND);
        this->DrawLevelSelector();
        this->DrawReturnSign();
    }
    else
    {
        this->DrawBackground(MAIN_MENU_BACKGROUND);
        this->DrawPlay();
        this->DrawLevel();
    }
    SDL_RenderPresent(renderer.get());
}

void Display::DrawVictory()
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    dest.w = 96 * BUTTON_ZOOM * 2;
    dest.h = 32 * BUTTON_ZOOM * 2;
    dest.x = (window_w - dest.w)/2;
    dest.y = (window_h - dest.h)/2;
    TextureManager::Draw(renderer.get(), textures[VICTORY_SIGN][game.get_active_player_id()].get(), &dest);
}

void Display::DrawBackground(int to_draw)
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    dest.w = window_w;
    dest.h = window_h;
    dest.x = dest.y = 0;
    if (to_draw == MAIN_MENU_BACKGROUND)
    {
        TextureManager::Draw(renderer.get(), textures[MAIN_MENU_BACKGROUND][0].get(), &dest);
    }
    else if (to_draw == LEVEL_SELECTOR_BACKGROUND)
    {
        TextureManager::Draw(renderer.get(), textures[LEVEL_SELECTOR_BACKGROUND][0].get(), &dest);
    }
}

void Display::DrawMap()
{
    int map_row_size = game.get_width();
    int map_col_size = game.get_height();
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
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
            TileManager::DrawTile(renderer.get(), textures, &dest, game.get_display_infos(coordinates(i,j)), game.get_display_infos(coordinates(i,j)).get_Tile().get_owner());
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
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
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
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    dest.w = BUTTON_X_SIZE;
    dest.h = BUTTON_Y_SIZE;
    dest.x = window_w/2 - BUTTON_SPACE/2 - BUTTON_X_SIZE;
    dest.y = window_h - BUTTON_Y_SIZE;
    TextureManager::Draw(renderer.get(), textures[REWIND_SIGN][0].get(), &dest);
    dest.x += BUTTON_X_SIZE + (BUTTON_SPACE - PROVINCE_PANEL_SIZE)/2;
    if (game.do_display_panel())
    {
        DrawProvincePanel();
    }
    dest.w = BUTTON_X_SIZE;
    dest.x += PROVINCE_PANEL_SIZE + (BUTTON_SPACE - PROVINCE_PANEL_SIZE)/2;
    TextureManager::Draw(renderer.get(), textures[END_TURN_SIGN][0].get(), &dest);
}

void Display::DrawProvincePanel()
{
    int player_turn_id = game.get_active_player_id();
    int to_restore = dest.x;
    int gold = game.get_displayed_gold();
    dest.w = PROVINCE_PANEL_SIZE;
    TextureManager::Draw(renderer.get(), textures[PROVINCE_PANEL][0].get(), &dest);
    dest.w = 32 * BUTTON_ZOOM;
    if (gold >= PEASANT_COST)
        DrawPanelButton(textures[PEASANT_TILE][player_turn_id].get(), PEASANT_COST, 0);
    dest.x += 32 * BUTTON_ZOOM;
    if (gold >= SOLDIER_COST)
        DrawPanelButton(textures[SOLDIER_TILE][player_turn_id].get(), SOLDIER_COST, 0);
    dest.x += 32 * BUTTON_ZOOM;
    if (gold >= KNIGHT_COST)
        DrawPanelButton(textures[KNIGHT_TILE][player_turn_id].get(), KNIGHT_COST, 0);
    dest.x += 32 * BUTTON_ZOOM;
    if (gold >= HERO_COST)
        DrawPanelButton(textures[HERO_TILE][player_turn_id].get(), HERO_COST, 0);
    dest.x += 32 * BUTTON_ZOOM;
    if (gold >= FORTRESS_COST)
        DrawPanelButton(textures[FORTRESS_TILE][player_turn_id].get(), FORTRESS_COST, 4 * BUTTON_ZOOM);
    dest.x += 36 * BUTTON_ZOOM;
    NumberManager::DrawMoneyRecap(renderer.get(), textures, &dest, game.get_displayed_gold(), game.get_displayed_income());
    dest.x = to_restore;
}

void Display::DrawPanelButton(SDL_Texture* to_render, int price, int tower_y_displacement)
{
    dest.y += tower_y_displacement;
    dest.y -= 5 * BUTTON_ZOOM;
    TextureManager::Draw(renderer.get(), to_render, &dest);
    dest.y += 27 * BUTTON_ZOOM;
    dest.x += (32 - 18) / 2 * BUTTON_ZOOM;
    dest.y -= tower_y_displacement;
    NumberManager::DrawNumbers(renderer.get(), textures, &dest, price);
    dest.x -= (32 - 18) / 2 * BUTTON_ZOOM;
    dest.y -= 22 * BUTTON_ZOOM;
}

void Display::DrawPlayerIndicator()
{
    int actu_player_turn = game.get_active_player_id();
    int next_player_turn = game.get_next_player_id();
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    dest.w = 32 * BUTTON_ZOOM;
    dest.h = 32 * BUTTON_ZOOM;
    dest.x = window_w - 32 * BUTTON_ZOOM - 32;
    dest.y = 10 * BUTTON_ZOOM;
    TextureManager::Draw(renderer.get(), textures[PLAYER_TURN_INDICATION][actu_player_turn].get(), &dest);
    dest.w = dest.w/2;
    dest.h = dest.h/2;
    dest.x += 20 * BUTTON_ZOOM;
    dest.y -= 4 * BUTTON_ZOOM;
    TextureManager::Draw(renderer.get(), textures[PLAYER_TURN_INDICATION][next_player_turn].get(), &dest);
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
        TextureManager::Draw(renderer.get(), textures[FORTRESS_TILE][game.get_active_player_id()].get(), &dest);
    }
    switch (to_draw.get_Tile().get_character().get_type())
    {
        case Peasant:
            TextureManager::Draw(renderer.get(), textures[PEASANT_TILE][game.get_active_player_id()].get(), &dest);
            break;
        case Soldier:
            TextureManager::Draw(renderer.get(), textures[SOLDIER_TILE][game.get_active_player_id()].get(), &dest);
            break;
        case Knight:
            TextureManager::Draw(renderer.get(), textures[KNIGHT_TILE][game.get_active_player_id()].get(), &dest);
            break;
        case Hero:
            TextureManager::Draw(renderer.get(), textures[HERO_TILE][game.get_active_player_id()].get(), &dest);
            break;
    }
}

void Display::DrawReturnSign()
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    dest.w = BUTTON_X_SIZE/2;
    dest.h = BUTTON_Y_SIZE/2;
    dest.x = 32 /2;
    dest.y = 32 /2;
    TextureManager::Draw(renderer.get(), textures[RETURN_SIGN][0].get(), &dest);
}

bool Display::InReturnSign(int posx, int posy)
{
    if (!nowPlaying and !inLevelSelection)
    {
        return false;
    }
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    dest.w = BUTTON_X_SIZE/2;
    dest.h = BUTTON_Y_SIZE/2;
    dest.x = 32 /2;
    dest.y = 32 /2;
    int mouse_x_in_ref = posx - dest.x;
    int mouse_y_in_ref = posy - dest.y;
    return (mouse_x_in_ref >= 0 and mouse_x_in_ref <= dest.w and mouse_y_in_ref >= 0 and mouse_y_in_ref <= dest.h);
}

bool Display::InProvincePanel(int posx, int posy, int *tile_on)
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
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
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
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
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    dest.w = PLAY_X_SIZE * 5;
    dest.h = PLAY_Y_SIZE * 5;
    dest.x = window_w/2 - dest.w/2;
    dest.y = window_h/2 - dest.h/2;
    TextureManager::Draw(renderer.get(), textures[PLAY_BUTTON][0].get(), &dest);
}

void Display::DrawLevel()
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    dest.w = PLAY_X_SIZE * 2;
    dest.h = PLAY_Y_SIZE * 2;
    dest.x = window_w/2 - dest.w/2;
    dest.y = window_h/2 - dest.h/2 + PLAY_Y_SIZE * 5;
    TextureManager::Draw(renderer.get(), textures[LEVEL_BUTTON][0].get(), &dest);
}

void Display::DrawLevelSelector()
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    dest.w = 32 * BUTTON_ZOOM;
    dest.h = 32 * BUTTON_ZOOM;
    dest.x = (window_w - 5 * dest.w)/6;
    dest.y = (window_h - dest.h * (LEVEL_NUMBER/5))/2;
    for (int i = 1; i <= LEVEL_NUMBER; i++)
    {
        TextureManager::Draw(renderer.get(), textures[ONE_LEVEL_BACKGROUND][0].get(), &dest);
        NumberManager::DrawOneNumber(renderer.get(), textures, &dest, i);
        dest.x += dest.w + (window_w - 5 * dest.w)/6;
        if (i%5 == 0)
        {
            dest.x = (window_w - 5 * dest.w)/6;
            dest.y += dest.h * 2;
        }
    }
}

bool Display::InLevelSelector(int posx, int posy, int *level_selected)
{
    if (nowPlaying or !inLevelSelection)
    {
        return false;
    }
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
    dest.w = 32 * BUTTON_ZOOM;
    dest.h = 32 * BUTTON_ZOOM;
    dest.x = (window_w - 5 * dest.w)/6;
    dest.y = (window_h - dest.h * (LEVEL_NUMBER/5))/2;
    for (int i = 1; i <= LEVEL_NUMBER; i++)
    {
        if (posx - dest.x >= 0 and posx - dest.x <= dest.w and posy - dest.y >= 0 and posy - dest.y <= dest.h)
        {
            *level_selected = i;
            return true;
        }
        dest.x += dest.w + (window_w - 5 * dest.w)/6;
        if (i%5 == 0)
        {
            dest.x = (window_w - 5 * dest.w)/6;
            dest.y += dest.h * 2;
        }
    }
    return false;
}

bool Display::InLevel(int posx, int posy)
{
    if (nowPlaying)
    {
        return false;
    }
    int window_w;
    int window_h;
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
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
    SDL_GetWindowSize(window.get(), &window_w, &window_h);
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
    SDL_Quit();
    std::cout << "Display Killed" << std::endl;
}