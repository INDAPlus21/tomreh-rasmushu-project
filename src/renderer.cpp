#include <iostream>
#include <vector>
#include "renderer.h"
#include <GLFW/glfw3.h>
#include "exceptions.h"

#include "shader.h"
#include "game.h"
#include "keyboard.h"
#include "mouse.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

const char *vertexShaderSource = "res/shaders/vertex.vert";
const char *fragmentShaderSource = "res/shaders/fragment.frag";
static GLFWwindow* s_window = nullptr;

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
    
    int monitor_count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);
    printf("\nMonitor count: %d\n", monitor_count);
    const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    printf("Color format: %d bit RGB\n\n", mode->redBits + mode->greenBits + mode->blueBits);
    printf("Refresh rate: %d Hz\n\n", mode->refreshRate);

    s_window = glfwCreateWindow(mode->width, mode->height, "Rayman", monitors[0], NULL);

    //s_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rayman", NULL, NULL);


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

    keyboard_init(s_window);
    mouse_init(s_window);
    mouse_position_set(256.0f, 256.0f);

    return true;
}

static GLint cam_pos_location = 0;
static GLint target_pos_location = 0;
static GLint power_location = 0;

void CreateThings() 
{
    unsigned int va;
    unsigned int vb;
    unsigned int ib;

    float verts[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    unsigned int indices[6] = {
        0, 1, 2,
        3, 2, 0
    };

    Layout layout = {0, std::vector<VertexBufferElement>()};
    GenVertexArray(&va);
    GenVertexBuffer(&vb, verts, 4 * 3 * sizeof(float));
    AddToLayout(layout, GL_FLOAT, 3);
    ConfigVertexArrayLayout(&va, &vb, layout);

    GenIndexBuffer(&ib, indices, 6);

    GLuint program = CreateProgram(vertexShaderSource, fragmentShaderSource);
    glUseProgram(program);

    cam_pos_location = glGetUniformLocation(program, "camera_position");
    target_pos_location = glGetUniformLocation(program, "target_position");
    power_location = glGetUniformLocation(program, "fractal_power");
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

    else if (keyboard_is_pressed(GLFW_KEY_ESCAPE))
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

void GenVertexBuffer(unsigned int *id, const void* data, unsigned int size) 
{
    glGenBuffers(1, id);
    glBindBuffer(GL_ARRAY_BUFFER, *id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void DeleteVetexBuffer(unsigned int *id)
{
    glDeleteBuffers(GL_ARRAY_BUFFER, id);
}

void GenVertexArray(unsigned int *id)
{
    glGenVertexArrays(1, id);
}

void DeleteVertexArray(unsigned int *id)
{
    glDeleteVertexArrays(1, id);
}

void GenIndexBuffer(unsigned int *id, const unsigned int *data, int count) 
{
    glGenBuffers(1, id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void DeleteIndexBuffer(unsigned int *id)
{
    glDeleteBuffers(1, id);
}

void ConfigVertexArrayLayout(unsigned int *va, unsigned int *vb, const Layout &layout)
{
    glBindBuffer(GL_ARRAY_BUFFER, *vb);
    glBindVertexArray(*va);

    unsigned int offset = 0;
    for (unsigned int i = 0; i < layout.elements.size(); i++)
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

void AddToLayout(Layout &layout, GLuint type, unsigned int count, bool normalize)
{
    struct VertexBufferElement elem = {type, count, normalize};

    layout.elements.push_back(elem);
    layout.stride += count * VertexBufferElement::GetSizeOfType(type);
}
