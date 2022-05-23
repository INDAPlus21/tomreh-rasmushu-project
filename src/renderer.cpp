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

void deleteBuffers(uint32_t *vb, uint32_t *va, uint32_t *ib)
{
    glDeleteBuffers(1, vb);
    glDeleteBuffers(1, ib);
    glDeleteVertexArrays(1, va);
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

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    createFullscreenQuad(scene);

    return true;
}

void Renderer::renderer_prepare()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

bool Renderer::drawToScreen(Scene &scene)
{
        // HACK: Move to event handler later
    if (glfwWindowShouldClose(s_window))
    {
        return false;
    }

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

void Renderer::renderer_clean_up()
{
    glfwTerminate();
}
