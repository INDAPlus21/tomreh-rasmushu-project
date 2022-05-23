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

void Renderer::addToLayout(Layout &layout, GLuint type, uint32_t count, bool normalize = false)
{
    VertexBufferElement elem = {type, count, normalize};

    layout.elements.push_back(elem);
    layout.stride += count * VertexBufferElement::GetSizeOfType(type);
}

void Renderer::configVertexArrayLayout(uint32_t *va, uint32_t *vb, Layout &layout)
{
    glBindVertexArray(*va);
    glBindBuffer(GL_ARRAY_BUFFER, *vb);

    uint32_t offset = 0;
    for (uint32_t i = 0; i < layout.elements.size(); i++)
    {
        glVertexAttribPointer(i, layout.elements[i].count,
                                 layout.elements[i].type,
                                 layout.elements[i].normalized,
                                 layout.stride,
                                 (void*) offset);
        glEnableVertexAttribArray(i);
        offset += layout.elements[i].count * VertexBufferElement::GetSizeOfType(layout.elements[i].type);
    }
}

void Renderer::createFullscreenQuad(Scene &scene)
{
    float verts[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };

    uint32_t vb;
    uint32_t va;
    glGenBuffers(1, &vb);
    genVertexArray(&va);
    uint32_t program = CreateProgram(vertexShaderSource, fragmentShaderSource);

    glBindVertexArray(va);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    scene.fsq.vb_handle = vb;
    scene.fsq.va_handle = va;   
    scene.fsq.program_handle = program;
}

void Renderer::genVertexBuffer(uint32_t *id, const void* data, uint32_t size) 
{
    glGenBuffers(1, id);
    glBindBuffer(GL_ARRAY_BUFFER, *id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Renderer::deleteVetexBuffer(uint32_t *id)
{
    glDeleteBuffers(GL_ARRAY_BUFFER, id);
}

void Renderer::genVertexArray(uint32_t *id)
{
    glGenVertexArrays(1, id);
}

void Renderer::deleteVertexArray(uint32_t *id)
{
    glDeleteVertexArrays(1, id);
}

void Renderer::genIndexBuffer(uint32_t *id, const uint32_t *data, int count) 
{
    glGenBuffers(1, id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

void Renderer::deleteIndexBuffer(uint32_t *id)
{
    glDeleteBuffers(1, id);
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
    std::cout << "Clearing" << std::endl;
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
    glDrawArrays(GL_TRIANGLES, 0, 3);
    std::cout << "Drawing done!" << std::endl;
    return true;
}

void Renderer::renderer_present()
{
    glfwSwapBuffers(s_window);
    glfwPollEvents();

    std::cout << "Presenting" << std::endl;
}

void Renderer::renderer_clean_up()
{
    glfwTerminate();
}
