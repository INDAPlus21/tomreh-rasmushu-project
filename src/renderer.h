#pragma once

#define GLEW_STATIC 
#include <GL/glew.h>
#include "scene.h"

#define WINDOW_WIDTH 1240
#define WINDOW_HEIGHT 720

namespace Renderer
{
    void genBuffers(uint32_t *vb, 
                    uint32_t *va, 
                    const void* data,
                    size_t size,
                    std::vector<uint32_t> &layout);
    void deleteBuffers(uint32_t *vb, uint32_t *va);
    void createFullscreenQuad(Scene &scene);
    void initRenderObject(RenderData &object);
    void initFractalObject(FractalData &object);

    bool init(Scene &scene);
    bool render(Scene &scene);
    bool renderFractal(Scene &scene);
    void renderer_present();
    void renderer_clean_up(Scene &scene);
}


