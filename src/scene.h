#pragma once

#include <vector>
#include <cinttypes>

struct FullScreenQuad
{
    uint32_t va_handle;
    uint32_t vb_handle;
    uint32_t program_handle;
};

// TODO: add count of ib_handle
struct RenderData
{
    uint32_t va_handle;
    uint32_t vb_handle;
    uint32_t program_handle;
    uint32_t fb_handle;
    uint32_t out_tex_handle;
};

struct FractalData
{
    int fractal_id;
    float fractal_power;
    uint32_t va_handle;
    uint32_t vb_handle;
    uint32_t program_handle;
    uint32_t fb_handle;
    uint32_t out_tex_handle;
};

struct Scene
{
    std::vector<RenderData> render_list;
    std::vector<FractalData> fractal_list;
    FullScreenQuad fsq;
};