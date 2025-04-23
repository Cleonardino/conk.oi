#include "display.hpp"

int main(int argc, char *argv[])
{
    Display display;

    display.init("Coucou je suis la fenetre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, false);

    while (display.running()) {
        display.handleEvents();
        display.update();
        display.render();
    }

    display.clean();

    return 0;
}