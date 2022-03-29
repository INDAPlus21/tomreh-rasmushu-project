#include "game.h"

#include <thread>
#include <iostream>
#include <chrono>

#include "renderer.h"

static bool s_running = false;

bool game_init()
{
	std::cout << "Initializing game" << std::endl;

	if (!renderer_init())
	{
		return false;
	}

	return true;
}

void game_run()
{
	std::cout << "Running game" << std::endl;

	s_running = true;

	while (s_running)
	{
		renderer_prepare();
		renderer_render();
		renderer_present();

		// HACK: Delta time should be calculated correctly
		std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(16));
	}
}

void game_close()
{
	std::cout << "Closing game" << std::endl;
	s_running = false;
}

void game_clean_up()
{
	renderer_clean_up();
}
