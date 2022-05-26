#include "mouse.h"

static mouse_data s_data;

void mouse_init(GLFWwindow* window)
{
	s_data.window = window;
	glfwSetInputMode(s_data.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	s_data.position_x = 0.0f;
	s_data.position_y = 0.0f;
}

float mouse_position_get_x()
{
	double x, y;
	glfwGetCursorPos(s_data.window, &x, &y);

	return (float)x;
}

float mouse_position_get_y()
{
	double x, y;
	glfwGetCursorPos(s_data.window, &x, &y);

	return (float)y;
}

void mouse_position_set(float x, float y)
{
	s_data.position_x = x;
	s_data.position_y = y;
	glfwSetCursorPos(s_data.window, (double)x, (double)y);
}

bool mouse_is_left()
{
	int state = glfwGetMouseButton(s_data.window, GLFW_MOUSE_BUTTON_LEFT);
	return state == GLFW_PRESS;
}

bool mouse_is_right()
{
	int state = glfwGetMouseButton(s_data.window, GLFW_MOUSE_BUTTON_RIGHT);
	return state == GLFW_PRESS;
}

bool mouse_is_middle()
{
	int state = glfwGetMouseButton(s_data.window, GLFW_MOUSE_BUTTON_MIDDLE);
	return state == GLFW_PRESS;
}
