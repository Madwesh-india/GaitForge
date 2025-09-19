#include "line.hpp"

Line::Line(double length, double thickness, Screen &context):
_length(length), _thickness(thickness), _context(context)
{
    link_pose = ComplexD(0, 0);
    local_point_2 = _length*std::exp(ComplexD(0, 0));

    const_curve_points_1.resize(CURVE_RESOLUTION);
    for (auto [i, theta] : std::views::enumerate(range_n_spaced(CURVE_RESOLUTION, M_PI / 2, 3 * M_PI / 2))) {
        const_curve_points_1(i) = _thickness * std::exp(ComplexD(0, theta));
    }

    const_curve_points_2.resize(CURVE_RESOLUTION);
    for (auto [i, theta] : std::views::enumerate(range_n_spaced(CURVE_RESOLUTION, M_PI / 2, -M_PI / 2))) {
        const_curve_points_2(i) = _thickness * std::exp(ComplexD(0, theta));
    }

    offset = ComplexD(_context.rect.w/2, _context.rect.h/2);

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

Line::~Line()
{
    
}

bool Line::update()
{
    rot_trasform = std::exp(ComplexD(0, M_PI_2));
    global_point_1 = link_pose; 
    global_point_2 = (rot_trasform * local_point_2) + link_pose;

    return true;
}

bool Line::draw()
{
    pxl_render_points_1 = _context._m_2_pxl_scale * ((rot_trasform * const_curve_points_1).array() + global_point_1).conjugate() + offset; 
    pxl_render_points_2 = _context._m_2_pxl_scale * ((rot_trasform * const_curve_points_2).array() + global_point_2).conjugate() + offset;    

    for (int i=0;i<CURVE_RESOLUTION;i++){
        _verts[i].position.x = pxl_render_points_1[i].real();
        _verts[i].position.y = pxl_render_points_1[i].imag();

        _verts[i + CURVE_RESOLUTION].position.x = pxl_render_points_2[i].real();
        _verts[i + CURVE_RESOLUTION].position.y = pxl_render_points_2[i].imag();

        _verts[i].color.r = ((float)127)/(255.f);
        _verts[i].color.g = ((float)22)/(255.f);
        _verts[i].color.b = ((float)76)/(255.f);
        _verts[i].color.a = 0.5;

        _verts[i + CURVE_RESOLUTION].color.r = ((float)127)/(255.f);
        _verts[i + CURVE_RESOLUTION].color.g = ((float)22)/(255.f);
        _verts[i + CURVE_RESOLUTION].color.b = ((float)76)/(255.f);
        _verts[i + CURVE_RESOLUTION].color.a = 0.5;
    }

    SDL_RenderGeometry(_context.getRenderer(), nullptr, _verts, CURVE_POINTS, indices, CURVE_INDEXS);
    return true;
}