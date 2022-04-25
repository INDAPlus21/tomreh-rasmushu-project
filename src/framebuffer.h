#pragma once

void GenRenderBuffer(unsigned int *id, unsigned int width, unsigned int height);
void DeleteRenderBuffer(unsigned int *id);
void GenTexture2D(unsigned int *id, unsigned int width, unsigned int height);
void DeleteTexture2D(unsigned int *id);
void GenFrameBuffer(uint *id, const uint rb, const uint tx);
