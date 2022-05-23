#include <iostream>
#include <vector>
#include "renderer.h"
#include <GLFW/glfw3.h>
#include "exceptions.h"

#include "shader.h"
#include "framebuffer.h"

using namespace Renderer;

const char *vertexShaderSource = "res/shaders/vertex.vert";
const char *fragmentShaderSource = "res/shaders/fragment.frag";
const char *ppFragShaderSource = "res/shaders/pp.frag";
const char *ppVertShaderSource = "res/shaders/pp.vert";

static GLFWwindow* s_window = nullptr;

static void glfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}

void Renderer::createFullscreenQuad(Scene &scene)
{
    float verts[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        3, 2, 0
    };

    std::vector<uint32_t> layout;
    // Maybe error?
    layout.push_back(2);
    layout.push_back(2);

    uint32_t vb;
    uint32_t ib;
    uint32_t va;
    genBuffers(&vb, &va, &ib, verts, sizeof(verts), indices, sizeof(indices), layout);
    uint32_t program = CreateProgram(ppVertShaderSource, ppFragShaderSource);

    scene.fsq.vb_handle = vb;
    scene.fsq.va_handle = va;
    scene.fsq.program_handle = program;
}

void Renderer::genBuffers(uint32_t *vb, uint32_t *va, uint32_t *ib,
                          const void* data, size_t size,
                          const void* indices, size_t i_size,
                          std::vector<uint32_t> &layout)
{
    glGenBuffers(1, vb);
    glGenBuffers(1, ib);
    glGenVertexArrays(1, va);

    glBindVertexArray(*va);
    glBindBuffer(GL_ARRAY_BUFFER, *vb);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size, indices, GL_STATIC_DRAW);
    uint32_t offset = 0;
    uint32_t i = 0;
    for (uint32_t elem : layout)
    {
        glVertexAttribPointer(i, elem, GL_FLOAT, GL_FALSE, 4 * elem, (void*) offset);
        glEnableVertexAttribArray(i);
        offset += elem * 4;
        i++;
    }
}

void Renderer::deleteBuffers(uint32_t *vb, uint32_t *va, uint32_t *ib)
{
    glDeleteBuffers(1, vb);
    glDeleteBuffers(1, ib);
    glDeleteVertexArrays(1, va);
}

void Renderer::initRenderObject(RenderData &object)
{
    float verts[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        3, 2, 0
    };

    std::vector<uint32_t> layout;
    layout.push_back(3);

    uint32_t vb;
    uint32_t ib;
    uint32_t va;
    genBuffers(&vb, &va, &ib, verts, sizeof(verts), indices, sizeof(indices), layout);
    uint32_t program = CreateProgram(vertexShaderSource, fragmentShaderSource);

    uint32_t tex;
    uint32_t rb;
    uint32_t fb;

    // TODO: borde inte vara fullscreen
    genTexture2D(&tex, WINDOW_WIDTH, WINDOW_HEIGHT);
    genRenderBuffer(&rb, WINDOW_WIDTH, WINDOW_HEIGHT);
    genFrameBuffer(&fb, rb, tex);

    object.vb_handle = vb;
    object.ib_handle = ib;
    object.va_handle = va;   
    object.program_handle = program;
    object.fb_handle = fb;
    object.out_tex_handle = tex;
}

bool Renderer::renderer_init(Scene &scene)
{
    std::cout << "Initializing renderer" << std::endl;

    glfwSetErrorCallback(&glfwError);

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    std::cout << "I'm apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    s_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rayman", NULL, NULL);


    if (!s_window)
    {
        std::cout << "Window could not be created" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(s_window);

    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Glew could not be initialized" << std::endl;
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    createFullscreenQuad(scene);

    return true;
}

void Renderer::renderer_prepare()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderObject(RenderData &object)
{
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object.fb_handle);
    glUseProgram(object.program_handle);
    glBindVertexArray(object.va_handle);
    // TODO: 6 should be ib.count ish
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

bool Renderer::drawToScreen(Scene &scene)
{
        // HACK: Move to event handler later
    if (glfwWindowShouldClose(s_window))
    {
        return false;
    }

    glDisable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    //TODO: make 1 not fixed
    glBindTexture(GL_TEXTURE_2D, 1);
    glUseProgram(scene.fsq.program_handle);
    glBindVertexArray(scene.fsq.va_handle);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    return true;
}

void Renderer::renderer_present()
{
    glfwSwapBuffers(s_window);
    glfwPollEvents();
}

void Renderer::renderer_clean_up(Scene &scene)
{
    for (RenderData data : scene.render_list)
    {
        deleteBuffers(&data.vb_handle, &data.va_handle, &data.ib_handle);
    }

    deleteBuffers(&scene.fsq.vb_handle, &scene.fsq.va_handle, &scene.fsq.ib_handle);
    glfwTerminate();
}
