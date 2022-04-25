#include "framebuffer.h"
#include "renderer.h"

typedef unsigned int uint;

void GenRenderBuffer(uint *id, uint width, uint height)
{
    glGenRenderbuffers(1, id);
    glBindRenderbuffer(GL_RENDERBUFFER, *id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height);
}

void DeleteRenderBuffer(uint *id)
{
    glDeleteRenderbuffers(1, id);
}

void GenTexture2D(uint *id, uint width, uint height)
{
    glGenTextures(1, id);
    glBindTexture(GL_TEXTURE_2D, *id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

void DeleteTexture2D(uint *id)
{
    glDeleteTextures(1, id);
}

void GenFrameBuffer(uint *id, const uint rb, const uint tx)
{
    glGenFramebuffers(1, id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tx, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb);
}
