#include <render_utils.hpp>
#include <iostream>

Screen::Screen(const char *title, int w, int h, SDL_WindowFlags flags): 
window(SDL_CreateWindow(title, w, h, flags), SDL_DestroyWindow),
renderer(SDL_CreateRenderer(window.get(), NULL), SDL_DestroyRenderer)
{
    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_GetRenderOutputSize(renderer.get(), &w, &h);
    SDL_SetRenderScale(renderer.get(), 1., 1.);
}

Screen::~Screen()
{
    SDL_DestroyRenderer(renderer.get());
    SDL_DestroyWindow(window.get());
    SDL_Quit();
}

bool Screen::loop()
{
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return false;
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
            return false;
        }
    }
    
    for(line_t object:objects){
        SDL_SetRenderDrawColor(renderer.get(), object.r, object.g, object.b, 127);  
        SDL_RenderLine(renderer.get(), object.x1, object.y1, object.x2, object.y2);
    }

    SDL_Delay(16); // ~60 FPS throttle
    return true;
}

bool Screen::addObjects(line_t line)
{
    objects.push_back(line);
    return true;
}
