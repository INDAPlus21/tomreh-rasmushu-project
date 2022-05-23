#pragma once

void genRenderBuffer(unsigned int *id, unsigned int width, unsigned int height);
void deleteRenderBuffer(unsigned int *id);
void genTexture2D(unsigned int *id, unsigned int width, unsigned int height);
void deleteTexture2D(unsigned int *id);
void genFrameBuffer(unsigned int *id, const unsigned int rb, const unsigned int tx);
