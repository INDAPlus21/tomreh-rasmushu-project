#pragma once

#define GLEW_STATIC 
#include <GL/glew.h>
#include <vector>
#include <cstdint>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef unsigned int uint;

namespace Renderer
{
    struct FullScreenQuad
    {
        uint32_t va_handle;
        uint32_t vb_handle;
        uint32_t ib_handle;
        uint32_t program_handle;
    };

    // TODO: add count of ib_handle
    struct RenderData
    {
        uint32_t va_handle;
        uint32_t vb_handle;
        uint32_t ib_handle;
        uint32_t program_handle;
        uint32_t fb_handle;
        uint32_t out_tex_handle;
    };

    FullScreenQuad fsq;

    bool renderer_init();
    void renderer_prepare();
    void renderObject(RenderData &object);
    void renderer_present();
    void renderer_clean_up();

    void addRenderObject(RenderData &object);
    void drawToScreen();

    void createFullscreenQuad();
    void renderObject(RenderData &object);
    void genVertexBuffer(uint *id, const void* data, uint size);
    void deleteVetexBuffer(unsigned int *id);
    void genVertexArray(unsigned int *id);
    void deleteVertexArray(unsigned int *id);
    void genIndexBuffer(unsigned int *id, const unsigned int *data, int count);
    void deleteIndexBuffer(unsigned int *id);
}


