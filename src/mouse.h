#pragma once

#include <GLFW/glfw3.h>

struct mouse_data
{
	GLFWwindow* window;
	float position_x;
	float position_y;
};

void mouse_init(GLFWwindow* window);
float mouse_position_get_x();
float mouse_position_get_y();

void mouse_position_set(float x, float y);

bool mouse_is_left();
bool mouse_is_right();
bool mouse_is_middle();