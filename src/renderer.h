#pragma once

#define GLEW_STATIC 
#include <GL/glew.h>
#include <vector>
#include <cstdint>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef unsigned int uint;

struct FullScreenQuad
{
    uint32_t va_handle;
    uint32_t vb_handle;
    uint32_t ib_handle;
    uint32_t program_handle;
};

struct RenderData
{
    uint32_t va_handle;
    uint32_t vb_handle;
    uint32_t ib_handle;
    uint32_t program_handle;
    uint32_t fb_handle;
};

FullScreenQuad fsq;

bool renderer_init();
void renderer_prepare();
void renderObject(RenderData object);
void renderer_present();
void renderer_clean_up();


void CreateThings();
void GenVertexBuffer(unsigned int *id, const void* data, unsigned int size);
void DeleteVetexBuffer(unsigned int *id);
void GenVertexArray(unsigned int *id);
void DeleteVertexArray(unsigned int *id);
void GenIndexBuffer(unsigned int *id, const unsigned int *data, int count);
void DeleteIndexBuffer(unsigned int *id);
