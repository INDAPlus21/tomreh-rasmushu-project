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
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    std::vector<uint32_t> layout;
    // Maybe error?
    layout.push_back(2);
    layout.push_back(2);

    uint32_t vb;
    uint32_t va;
    genBuffers(&vb, &va, verts, sizeof(verts), layout);
    uint32_t program = CreateProgram(ppVertShaderSource, ppFragShaderSource);

    scene.fsq.vb_handle = vb;
    scene.fsq.va_handle = va;
    scene.fsq.program_handle = program;
}

void Renderer::genBuffers(uint32_t *vb, uint32_t *va,
                          const void* data, size_t size,
                          std::vector<uint32_t> &layout)
{
    glGenBuffers(1, vb);
    glGenVertexArrays(1, va);

    glBindVertexArray(*va);
    glBindBuffer(GL_ARRAY_BUFFER, *vb);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    uint32_t sum = 0;
    for (uint32_t val : layout)
    {
        sum += val;
    }

    uint32_t offset = 0;
    uint32_t i = 0;
    for (uint32_t elem : layout)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, elem, GL_FLOAT, GL_FALSE, 4 * sum, (void*) offset);
        offset += elem * 4;
        i++;
    }
}

void Renderer::deleteBuffers(uint32_t *vb, uint32_t *va)
{
    glDeleteBuffers(1, vb);
    glDeleteVertexArrays(1, va);
}

void Renderer::initRenderObject(RenderData &object)
{
    float verts[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
    };

    std::vector<uint32_t> layout;
    layout.push_back(3);

    uint32_t vb;
    uint32_t va;
    genBuffers(&vb, &va, verts, sizeof(verts), layout);
    uint32_t program = CreateProgram(vertexShaderSource, fragmentShaderSource);

    uint32_t tx;
    uint32_t rb;
    uint32_t fb;

    genFrameBuffer(&fb, &rb, &tx, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    object.vb_handle = vb;
    object.va_handle = va;   
    object.program_handle = program;
    object.fb_handle = fb;
    object.out_tex_handle = tx;
}

bool Renderer::init(Scene &scene)
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

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    createFullscreenQuad(scene);

    return true;
}

bool Renderer::render(Scene &scene)
{
    if (glfwWindowShouldClose(s_window))
    {
        return false;
    }

    //TODO: Add multi object funtionallity
    RenderData obj = scene.render_list.at(0);
    glBindFramebuffer(GL_FRAMEBUFFER, obj.fb_handle);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(obj.program_handle);

    glBindVertexArray(obj.va_handle);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(scene.fsq.program_handle);
    glBindVertexArray(scene.fsq.va_handle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj.out_tex_handle);

    glDrawArrays(GL_TRIANGLES, 0, 6);
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
        deleteBuffers(&data.vb_handle, &data.va_handle);
    }

    deleteBuffers(&scene.fsq.vb_handle, &scene.fsq.va_handle);
    glfwTerminate();
}
