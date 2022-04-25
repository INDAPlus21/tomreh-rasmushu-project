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

typedef struct DrawObject
{
    uint va;
    uint vb;
    uint ib;
    bool pp;
    uint *draw_program;
    uint *fb;
    Layout layout;
};

typedef struct FullscreenQuad
{
    uint va;
    uint vb;
    uint ib;
};

enum ObjectType
{
    triangle,
    rectangle,
    circle
};

static void glfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}

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

    return true;
}

// x, y, width, height, ska vara i pixel koordinater
DrawObject CreateDrawObject(ObjectType type, uint x, uint y, bool post_process,  uint width = 0, uint height = 0)
{
    uint va, vb, ib, program;
    float *verts;
    uint *indices;
    Layout layout = {0, std::vector<VertexBufferElement>()};

    if (type == rectangle)
    {
        verts = GenRectVertex(0, 0, 255, 255);
        indices = GenRectIndex();
    }

    GenVertexArray(&va);
    GenVertexBuffer(&vb, verts, 4 * 3 * sizeof(float));
    AddToLayout(layout, GL_FLOAT, 3);
    ConfigVertexArrayLayout(&va, &vb, layout);
    GenIndexBuffer(&ib, indices, 6);
    program = CreateProgram(vertexShaderSource, fragmentShaderSource);
    uint programs[2];
    programs[0] = program;
    uint fb_info[3] = {0, 0, 0};

    if (post_process)
    {  
        // Change shader sources to correct ones
        uint pp_program = CreateProgram(vertexShaderSource, fragmentShaderSource);
        programs[1] = pp_program;
        GenRenderBuffer(&fb_info[0], WINDOW_WIDTH, WINDOW_HEIGHT);
        GenTexture2D(&fb_info[1], WINDOW_WIDTH, WINDOW_HEIGHT);
        GenFrameBuffer(&fb_info[2], fb_info[0], fb_info[1]);
    }

    DrawObject returnthing = {va, vb, ib, post_process, programs, fb_info, layout};
}

void DrawDrawObject(DrawObject &object)
{

}

FullscreenQuad GenFullscreenRect()
{
    float verts[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };

        uint indices[] = {
        0, 1, 2,
        3, 2, 0
    };

    
}

void renderer_prepare()
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void renderer_render()
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

void GenVertexBuffer(uint *id, const void* data, uint size) 
{
    glGenBuffers(1, id);
    glBindBuffer(GL_ARRAY_BUFFER, *id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void DeleteVetexBuffer(uint *id)
{
    glDeleteBuffers(GL_ARRAY_BUFFER, id);
}

void GenVertexArray(uint *id)
{
    glGenVertexArrays(1, id);
}

void DeleteVertexArray(uint *id)
{
    glDeleteVertexArrays(1, id);
}

void GenIndexBuffer(uint *id, const uint *data, int count) 
{
    glGenBuffers(1, id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW);
}

void DeleteIndexBuffer(uint *id)
{
    glDeleteBuffers(1, id);
}

void ConfigVertexArrayLayout(uint *va, uint *vb, const Layout &layout)
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

void AddToLayout(Layout &layout, GLuint type, uint count, bool normalize = false)
{
    struct VertexBufferElement elem = {type, count, normalize};

    layout.elements.push_back(elem);
    layout.stride += count * VertexBufferElement::GetSizeOfType(type);
}
