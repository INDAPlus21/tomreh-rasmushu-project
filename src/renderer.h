#pragma once

#define GLEW_STATIC 
#include <GL/glew.h>
#include "scene.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

namespace Renderer
{
    bool renderer_init(Scene &scene);
    void renderer_prepare();
    void renderObject(RenderData &object);
    void renderer_present();
    void renderer_clean_up();

    struct VertexBufferElement
    {
        uint32_t type;
        uint32_t count;
        unsigned char normalized;

        static uint32_t GetSizeOfType(uint32_t type)
        {
            switch (type)
            {
                case GL_FLOAT: return 4;
                case GL_UNSIGNED_INT: return 4;
                case GL_UNSIGNED_BYTE: return 1;
            }
            return 0;
        }
    };

    bool drawToScreen(Scene &scene);
    void genBuffers(uint32_t *vb, 
                    uint32_t *va, 
                    uint32_t *ib, 
                    const void* data,
                    size_t size,
                    const void* indices,
                    size_t i_size, 
                    std::vector<uint32_t> &layout);
    void deleteBuffers(uint32_t vb, uint32_t va, uint32_t ib);
    void createFullscreenQuad(Scene &scene);
}


