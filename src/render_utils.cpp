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
    
    SDL_DisplayID display = SDL_GetDisplayForWindow(window.get());

    SDL_GetDisplayBounds(display, &rect);
    std::cout << "Display size: " << rect.w << " x " << rect.h << "\n";


    SDL_SetRenderScale(renderer.get(), 1., 1.);
    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
    
    int i = 0;
    int array_index = 0;
    while (array_index<CURVE_INDEXS){
        if (i < CURVE_RESOLUTION - 2){
            indices[array_index++] = i++;
            indices[array_index++] = i;
            indices[array_index++] = CURVE_RESOLUTION-1;
        }
        else if (i == CURVE_RESOLUTION - 2) {
            indices[array_index++] = 0;
            indices[array_index++] = CURVE_RESOLUTION-1;
            indices[array_index++] = CURVE_RESOLUTION;
            i+=1;
        }
        else if (i < CURVE_RESOLUTION*2-2){
            indices[array_index++] = i++;
            indices[array_index++] = i;
            indices[array_index++] = 2*CURVE_RESOLUTION-1;
        }
    }
}

Screen::~Screen()
{
    SDL_DestroyRenderer(renderer.get());
    SDL_DestroyWindow(window.get());
    SDL_Quit();
}

void Screen::_get_vertices(line_t &line)
{
    _aux_p1.x() = line.x1;
    _aux_p1.y() = line.y1;

    _aux_p2.x() = line.x2;
    _aux_p2.y() = line.y2; 

    Eigen::Vector2f dir = _aux_p2 - _aux_p1;
    Eigen::Vector2f perp(-dir.y(), dir.x());  // perpendicular vector
    
    perp.normalize();
    perp *= (thickness / 2.0f);

    Eigen::Vector2f points[2*CURVE_RESOLUTION];

    for (int i=0;i<CURVE_RESOLUTION;i++) {
        points[i] = _aux_p1 + this->_get_rotation(i*(M_PI/(CURVE_RESOLUTION-1)))*perp;
    }

    for (int i=0;i<CURVE_RESOLUTION;i++) {
        points[i + CURVE_RESOLUTION] = _aux_p2 + this->_get_rotation(-i*(M_PI/(CURVE_RESOLUTION-1)))*perp;
    }

    for (int i=0;i<CURVE_RESOLUTION*2;i++){
        _aux_verts[i].position.x = points[i].x();
        _aux_verts[i].position.y = points[i].y();
    }

    // set colors
    for (int i=0;i<CURVE_RESOLUTION*2;i++) {
        _aux_verts[i].color.r = ((float)line.r)/(255.f);
        _aux_verts[i].color.g = ((float)line.g)/(255.f);
        _aux_verts[i].color.b = ((float)line.b)/(255.f);
        _aux_verts[i].color.a = 0.5;
    }
}

Eigen::Matrix2f Screen::_get_rotation(float rad)
{
    Eigen::Matrix2f rot;
    rot << cos(rad), -sin(rad),
           sin(rad),  cos(rad);
    return rot;
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
        this->_get_vertices(object);
        SDL_RenderGeometry(renderer.get(), nullptr, _aux_verts, CURVE_POINTS, indices, CURVE_INDEXS);
        
        #ifdef DEBUG_MODE        
        std::vector<SDL_FPoint> debug_points;
        debug_points.reserve(CURVE_POINTS);

        for (int i = 0; i < CURVE_POINTS; i++) {
            debug_points.push_back(_aux_verts[i].position);
        }

        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
        SDL_RenderPoints(renderer.get(), debug_points.data(), debug_points.size());
        #endif
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