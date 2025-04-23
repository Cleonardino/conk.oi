#include "display.hpp"

int main(int argc, char *argv[])
{
    Display display;

    const int FPS = 20;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    display.init("Coucou je suis la fenetre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, false);

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