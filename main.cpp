#include "render_utils.hpp"
#include "line.hpp"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }
    const char title[] = "Test";
    Screen disp(title, 0, 0, 100);
    Line line1(1, 0.1, disp);
    
    disp.addObjects(line1);

    bool running = true;

    while (running) {
        running = disp.loop();
    }

    return 0;
}
