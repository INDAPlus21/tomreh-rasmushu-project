#pragma once

#define GLEW_STATIC 
#include <GL/glew.h>
#include <vector>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef unsigned int uint;

bool renderer_init();
void renderer_prepare();
void renderer_render();
void renderer_present();
void renderer_clean_up();


void CreateThings();
void GenVertexBuffer(unsigned int *id, const void* data, unsigned int size);
void DeleteVetexBuffer(unsigned int *id);
void GenVertexArray(unsigned int *id);
void DeleteVertexArray(unsigned int *id);
void GenIndexBuffer(unsigned int *id, const unsigned int *data, int count);
void DeleteIndexBuffer(unsigned int *id);
void ConfigVertexArrayLayout(unsigned int *va, unsigned int *vb, const Layout &layout);
void AddToLayout(Layout &layout, GLuint type, unsigned int count, bool normalize);
