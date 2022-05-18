#pragma once

#include <GLFW/glfw3.h>
#include <bitset>

struct keyboard_data
{
	GLFWwindow* window;
	std::bitset<1024> keys;
};

void keyboard_init(GLFWwindow* window);
bool keyboard_is_pressed(int key);
void keyboard_state_set(int key, bool state);

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);