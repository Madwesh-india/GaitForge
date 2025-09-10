#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <memory>
#include <vector>
#include <eigen3/Eigen/Core>

constexpr int indices[6] = {0, 1, 2,  
                          2, 1, 3};

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
        void get_vertices(line_t &line);
        

    private:
        WindowPtr window;
        RendererPtr renderer;
        SDL_Event event;

        std::vector<line_t> objects;

        int w, h;
        float thickness = 10;

        Eigen::Vector2f _aux_p1, _aux_p2;
        SDL_Vertex _aux_verts[4];
};