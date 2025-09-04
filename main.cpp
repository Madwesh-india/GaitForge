#include "render_utils.hpp"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }
    const char title[] = "Test";
    Screen disp(title, 800, 400);
    
    disp.addObjects(line_t{0.0f, 0.0f, 100.0f, 50.0f, 255, 0, 0});
    disp.addObjects(line_t{100.0f, 50.0f, 50.0f, 100.0f, 0, 255, 0});

    bool running = true;

    while (running) {
        running = disp.loop();
    }

    return 0;
}
