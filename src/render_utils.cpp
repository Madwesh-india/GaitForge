#include <render_utils.hpp>
#include "line.hpp"

Screen::Screen(const char *title, int w, int h, float m_2_pxl_scale, SDL_WindowFlags flags):
window(SDL_CreateWindow(title, w, h, flags), SDL_DestroyWindow), 
renderer(SDL_CreateRenderer(window.get(), NULL), SDL_DestroyRenderer),
_m_2_pxl_scale(m_2_pxl_scale)
{
    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }
    
    SDL_DisplayID display = SDL_GetDisplayForWindow(window.get());

    SDL_GetDisplayBounds(display, &rect);
    std::cout << "Display size: " << rect.w << " x " << rect.h << "\n";

    SDL_SetRenderScale(renderer.get(), 1., 1.);
    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
    
}

Screen::~Screen()
{
    SDL_Quit();
}

SDL_Renderer* Screen::getRenderer() const {
    return renderer.get();
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
    
    // Clear screen (set background color, e.g., black)
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.get());

    for(Line &object:objects){
        object.update();
        object.draw();
    }

    SDL_RenderPresent(renderer.get());
    SDL_Delay(16); // ~60 FPS throttle
    return true;
}

bool Screen::addObjects(Line &line)
{
    objects.push_back(line);
    return true;
}