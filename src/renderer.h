#pragma once

#define GLEW_STATIC 
#include <GL/glew.h>
#include "scene.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

namespace Renderer
{
    bool renderer_init(Scene scene);
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

    struct Layout
    {
        uint32_t stride;
        std::vector<VertexBufferElement> elements;
    };

    void addRenderObject(RenderData &object);
    void drawToScreen(Scene scene);

    void createFullscreenQuad(Scene scene);
    void renderObject(RenderData &object);
    void genVertexBuffer(uint32_t *id, const void* data, uint32_t size);
    void deleteVetexBuffer(unsigned int *id);
    void genVertexArray(unsigned int *id);
    void deleteVertexArray(unsigned int *id);
    void genIndexBuffer(unsigned int *id, const unsigned int *data, int count);
    void deleteIndexBuffer(unsigned int *id);
    void configVertexArrayLayout(uint32_t *va, uint32_t *vb, const Layout &layout);
    void addToLayout(Layout &layout, GLuint type, uint32_t count, bool normalize);
}


