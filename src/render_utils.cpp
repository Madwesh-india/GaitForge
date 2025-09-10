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
    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
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
    
    // Clear screen (set background color, e.g., black)
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.get());

    for(line_t object:objects){
        this->get_vertices(object);
        SDL_RenderGeometry(renderer.get(), nullptr, _aux_verts, 4, indices, 6);
    }

    SDL_RenderPresent(renderer.get());
    SDL_Delay(16); // ~60 FPS throttle
    return true;
}

bool Screen::addObjects(line_t line)
{
    objects.push_back(line);
    return true;
}

void Screen::get_vertices(line_t &line)
{
    _aux_p1.x() = line.x1;
    _aux_p1.y() = line.y1;

    _aux_p2.x() = line.x2;
    _aux_p2.y() = line.y2; 

    Eigen::Vector2f dir = _aux_p2 - _aux_p1;
    Eigen::Vector2f perp(-dir.y(), dir.x());  // perpendicular vector
    
    perp.normalize();
    perp *= (thickness / 2.0f);

    // four corners of the quad
    Eigen::Vector2f v0 = _aux_p1 + perp;
    Eigen::Vector2f v1 = _aux_p1 - perp;
    Eigen::Vector2f v2 = _aux_p2 + perp;
    Eigen::Vector2f v3 = _aux_p2 - perp;

    _aux_verts[0].position.x = v0.x();
    _aux_verts[0].position.y = v0.y();

    _aux_verts[1].position.x = v1.x();
    _aux_verts[1].position.y = v1.y();

    _aux_verts[2].position.x = v2.x();
    _aux_verts[2].position.y = v2.y();

    _aux_verts[3].position.x = v3.x();
    _aux_verts[3].position.y = v3.y();

    // set colors
    for (int i=0;i<4;i++) {
        _aux_verts[i].color.r = ((float)line.r)/(255.f);
        _aux_verts[i].color.g = ((float)line.g)/(255.f);
        _aux_verts[i].color.b = ((float)line.b)/(255.f);
        _aux_verts[i].color.a = 0.5;
    }
}
