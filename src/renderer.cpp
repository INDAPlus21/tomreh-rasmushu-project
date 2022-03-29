#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "renderer.h"

#include "game.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

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
    
    s_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rayman", NULL, NULL);
    if (!s_window)
    {
        std::cout << "Window could not be created" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(s_window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Glew could not be initialized" << std::endl;
        return false;
    }

    glClearColor(0.3f, 0.7f, 1.0f, 1.0f);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    return true;
}

void renderer_prepare()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_render()
{
    // HACK: Move to event handler later
    if (glfwWindowShouldClose(s_window))
    {
        game_close();
    }
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
