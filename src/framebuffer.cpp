#include "framebuffer.h"
#include "renderer.h"
#include <iostream>

void genFrameBuffer(uint32_t *fb, uint32_t *rb, uint32_t *tx, uint32_t width, uint32_t height)
{
    glGenFramebuffers(1, fb);
    glBindFramebuffer(GL_FRAMEBUFFER, *fb);

    glGenTextures(1, tx);
    glBindTexture(GL_TEXTURE_2D, *tx);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *tx, 0);

    glGenRenderbuffers(1, rb);
    glBindRenderbuffer(GL_RENDERBUFFER, *rb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *rb);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
	    std::cout << "Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void deleteFrameBuffer(uint32_t *fb, uint32_t *rb, uint32_t *tx)
{
    glDeleteFramebuffers(1, fb);
    glDeleteRenderbuffers(1, rb);
    glDeleteTextures(1, tx);
}