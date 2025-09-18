#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <memory>
#include <vector>
#include <eigen3/Eigen/Core>
#include <iostream>

class Line;

using WindowPtr   = std::unique_ptr<SDL_Window,   decltype(&SDL_DestroyWindow)>;
using RendererPtr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;

class Screen{
    public:
        Screen(const char *title, int w, int h, float m_2_pxl_scale, SDL_WindowFlags flags = SDL_WINDOW_FULLSCREEN);
        ~Screen();

        bool loop();
        bool addObjects(Line &line);
        
        SDL_Rect rect;
        const double _m_2_pxl_scale;

        SDL_Renderer* getRenderer() const;
        

    private:
        WindowPtr window;
        RendererPtr renderer;

        SDL_Event event;

        std::vector<Line> objects;
};