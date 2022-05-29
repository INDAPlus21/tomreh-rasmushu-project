#pragma once
#include <cinttypes>

void genFrameBuffer(uint32_t *fb, uint32_t *rb, uint32_t *tx, uint32_t width, uint32_t height);
void deleteFrameBuffer(uint32_t *fb, uint32_t *rb, uint32_t *tx);