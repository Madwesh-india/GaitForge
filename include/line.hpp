#pragma once
#include <SDL3/SDL.h>
#include <complex>
#include <memory>
#include <vector>
#include <eigen3/Eigen/Core>
#include <cmath>
#include <ranges>
#include "math_utilitys.hpp"
#include "render_utils.hpp"

class Screen;

constexpr int CURVE_RESOLUTION = 6;
constexpr int CURVE_POINTS = (CURVE_RESOLUTION*2);
constexpr int CURVE_INDEXS = (CURVE_RESOLUTION*2 - 2)*3;

using ComplexD = std::complex<double>;

class Line
{
    public:
        Line(double length, double thickness, Screen &context);
        ~Line();
        bool update();
        bool draw();

    private:
        double const _length, _thickness;
        Screen &_context;

        ComplexD link_pose, rot_trasform;
        ComplexD local_point_2;
        Eigen::VectorXcd const_curve_points_1, const_curve_points_2;

        ComplexD global_point_1, global_point_2;
        Eigen::VectorXcd global_curve_points_1, global_curve_points_2;

        Eigen::VectorXcd pxl_render_points_1, pxl_render_points_2;


        ComplexD offset;
        int indices[CURVE_INDEXS];
        SDL_Vertex _verts[CURVE_POINTS];
};
  