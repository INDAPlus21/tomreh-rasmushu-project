#include "primatives.h"
#include "renderer.h"

float *GenRectVertex(uint x, uint y, uint w, uint h)
{
    float center_x = x / WINDOW_WIDTH;
    float center_y = y / WINDOW_HEIGHT;
    float n_width = (w / WINDOW_WIDTH) / 2;
    float n_height = (h / WINDOW_HEIGHT) / 2;

    float verts[12] = {
        center_x - n_width, center_y - n_height, 0.0f,
        center_x + n_width, center_y - n_height, 0.0f,
        center_x + n_width, center_y + n_height, 0.0f,
        center_x - n_width, center_y + n_height, 0.0f
    };

    return verts;
}

//HACK: really dumb solution
uint *GenRectIndex(uint val = 0)
{
    uint indices[] = {
        val, 1, 2,
        3, 2, 0
    };

    return indices;
}
