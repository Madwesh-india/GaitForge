#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <memory>
#include <vector>

typedef struct {
    float x1;
    float y1;

    float x2;
    float y2;

    uint8_t r;
    uint8_t g;
    uint8_t b;
} line_t;

using WindowPtr   = std::unique_ptr<SDL_Window,   decltype(&SDL_DestroyWindow)>;
using RendererPtr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;

class Screen{
    public:
        Screen(const char *title, int w, int h, SDL_WindowFlags flags = SDL_WINDOW_FULLSCREEN);
        ~Screen();

        bool loop();
        bool addObjects(line_t line);

    private:
        WindowPtr window;
        RendererPtr renderer;
        SDL_Event event;

        std::vector<line_t> objects;

        int w, h;
};