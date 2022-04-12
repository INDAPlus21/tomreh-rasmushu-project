#pragma once

#define GLEW_STATIC 
#include <GL/glew.h>
#include <vector>

bool renderer_init();
void renderer_prepare();
void renderer_render();
void renderer_present();
void renderer_clean_up();

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
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
    unsigned int stride;
    std::vector<VertexBufferElement> elements;
};

void CreateThings();
void GenVertexBuffer(unsigned int *id, const void* data, unsigned int size);
void DeleteVetexBuffer(unsigned int *id);
void GenVertexArray(unsigned int *id);
void DeleteVertexArray(unsigned int *id);
void GenIndexBuffer(unsigned int *id, const unsigned int *data, int count);
void DeleteIndexBuffer(unsigned int *id);
void ConfigVertexArrayLayout(unsigned int *va, unsigned int *vb, const Layout &layout);
void AddToLayout(Layout &layout, GLuint type, unsigned int count, bool normalize = false);
