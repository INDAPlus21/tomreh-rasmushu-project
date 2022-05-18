#include <iostream>
#include <vector>
#include "renderer.h"
#include <GLFW/glfw3.h>
#include "exceptions.h"

#include "shader.h"
#include "game.h"
#include "framebuffer.h"
#include "primatives.h"

const char *vertexShaderSource = "res/shaders/vertex.vert";
const char *fragmentShaderSource = "res/shaders/fragment.frag";
const char *ppFragShaderSource = "res/shaders/pp.frag";
const char *ppVertShaderSource = "res/shaders/pp.vert";

static GLFWwindow* s_window = nullptr;

typedef struct VertexBufferElement
{
    uint type;
    uint count;
    unsigned char normalized;

    static uint GetSizeOfType(uint type)
    {
        switch (type)
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;
    }
};

typedef struct Layout
{
    uint stride;
    std::vector<VertexBufferElement> elements;
};

static void glfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}

void createFullscreenQuad()
{
    float verts[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };

    unsigned int indices[6] = {
        0, 1, 2,
        3, 2, 0
    };

    uint32_t vb;
    uint32_t ib;
    uint32_t va;
    genVertexBuffer(&vb, verts, 16 * sizeof(float));
    genIndexBuffer(&ib, indices, 6);
    genVertexArray(&va);
    uint32_t program = CreateProgram(ppVertShaderSource, ppFragShaderSource);

    Layout layout;
    addToLayout(layout, GL_FLOAT, 2);
    addToLayout(layout, GL_FLOAT, 2);
    configVertexArrayLayout(&va, &vb, layout);

    fsq.vb_handle = vb;
    fsq.ib_handle = ib;
    fsq.va_handle = va;   
    fsq.program_handle = program;
}

void initRenderObject(RenderData &obj)
{
    uint32_t vb;
    uint32_t ib;
    uint32_t va;
    uint32_t rb;
    uint32_t tex;
    uint32_t fb;

};

bool renderer_init()
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

    GL_CALL(glClearColor(0.3f, 0.7f, 1.0f, 1.0f));

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    createFullscreenQuad();

    return true;
}

void renderer_prepare()
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void renderObject(RenderData &object)
{
    glBindBuffer(GL_ARRAY_BUFFER, object.vb_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ib_handle);
    glBindVertexArray(object.va_handle);
    glUseProgram(object.program_handle);
}

void renderObject()
{
    // HACK: Move to event handler later
    if (glfwWindowShouldClose(s_window))
    {
        game_close();
    }



    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void renderer_present()
{
    glfwSwapBuffers(s_window);
    glfwPollEvents();
}

void renderer_clean_up()
{
    glfwTerminate();
}

void genVertexBuffer(uint *id, const void* data, uint size) 
{
    glGenBuffers(1, id);
    glBindBuffer(GL_ARRAY_BUFFER, *id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void deleteVetexBuffer(uint *id)
{
    glDeleteBuffers(GL_ARRAY_BUFFER, id);
}

void genVertexArray(uint *id)
{
    glGenVertexArrays(1, id);
}

void deleteVertexArray(uint *id)
{
    glDeleteVertexArrays(1, id);
}

void genIndexBuffer(uint *id, const uint *data, int count) 
{
    glGenBuffers(1, id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW);
}

void deleteIndexBuffer(uint *id)
{
    glDeleteBuffers(1, id);
}

void configVertexArrayLayout(uint *va, uint *vb, const Layout &layout)
{
    glBindBuffer(GL_ARRAY_BUFFER, *vb);
    glBindVertexArray(*va);

    uint offset = 0;
    for (uint i = 0; i < layout.elements.size(); i++)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, layout.elements[i].count,
                                 layout.elements[i].type,
                                 layout.elements[i].normalized,
                                 layout.stride,
                                 (const void*) offset);
        offset += layout.elements[i].count * VertexBufferElement::GetSizeOfType(layout.elements[i].type);
    }
    
}

void addToLayout(Layout &layout, GLuint type, uint count, bool normalize = false)
{
    struct VertexBufferElement elem = {type, count, normalize};

    layout.elements.push_back(elem);
    layout.stride += count * VertexBufferElement::GetSizeOfType(type);
}
