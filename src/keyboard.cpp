#include "keyboard.h"

static keyboard_data s_data;

void keyboard_init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, keyboard_callback);
}

bool keyboard_is_pressed(int key)
{
	return s_data.keys[key];
}

void keyboard_state_set(int key, bool state)
{
	s_data.keys[key] = state;
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keyboard_state_set(key, true);
	}
	
	else if (action == GLFW_RELEASE)
	{
		keyboard_state_set(key, false);
	}
}
