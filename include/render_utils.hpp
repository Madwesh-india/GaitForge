#pragma once
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <memory>
#include <vector>
#include <eigen3/Eigen/Core>

constexpr int CURVE_RESOLUTION = 6;
constexpr int CURVE_POINTS = (CURVE_RESOLUTION*2);
constexpr int CURVE_INDEXS = (CURVE_RESOLUTION*2 - 2)*3;

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
        Screen(const char *title, int w, int h, float m_2_pxl_scale, SDL_WindowFlags flags = SDL_WINDOW_FULLSCREEN);
        ~Screen();

        bool loop();
        bool addObjects(line_t line);
        

    private:
        WindowPtr window;
        RendererPtr renderer;
        const int _m_2_pxl_scale;

        SDL_Event event;

        std::vector<line_t> objects;

        SDL_Rect rect;
        float thickness = 10;

        Eigen::Matrix2f scale;
        Eigen::Vector2f offset;

        Eigen::Vector2f _aux_p1, _aux_p2;
        SDL_Vertex _aux_verts[CURVE_POINTS];

        void _get_vertices(line_t &line);
        Eigen::Matrix2f _get_rotation(float rad);

        Eigen::Vector2f _meters_2_screen(Eigen::Vector2f pos);

        int indices[CURVE_INDEXS];
};