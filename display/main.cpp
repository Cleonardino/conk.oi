#include "display.hpp"

int main(int argc, char *argv[])
{
    Map new_map = parse_csv("../example_map.txt");
    Game new_game = Game(new_map,0,std::vector<Province>());
    Display display = Display(new_game);

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    display.init("Coucou je suis la fenetre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 1000, false);

    while (display.running()) {
        frameStart = SDL_GetTicks();
        display.handleEvents();
        display.update();
        display.render();
        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    display.clean();

    return 0;
}