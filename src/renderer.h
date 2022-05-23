#pragma once

#define GLEW_STATIC 
#include <GL/glew.h>
#include "scene.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

namespace Renderer
{
    void genBuffers(uint32_t *vb, 
                    uint32_t *va, 
                    uint32_t *ib, 
                    const void* data,
                    size_t size,
                    const void* indices,
                    size_t i_size, 
                    std::vector<uint32_t> &layout);
    void deleteBuffers(uint32_t *vb, uint32_t *va, uint32_t *ib);
    void createFullscreenQuad(Scene &scene);
    void initRenderObject(RenderData &object);

    bool renderer_init(Scene &scene);
    void renderer_prepare();
    void renderObject(RenderData &object);
    bool drawToScreen(Scene &scene);
    void renderer_present();
    void renderer_clean_up(Scene &scene);
}


