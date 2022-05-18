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

	CreateThings();

	double delta_time = 1000.0 / 100.0f;
	std::chrono::system_clock::time_point last_frame = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> work_time;

	std::chrono::system_clock::time_point last_print = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
	std::chrono::duration<float, std::milli> since_last_print;

	int frames = 0;

	while (s_running)
	{
		last_frame = std::chrono::system_clock::now();

		renderer_prepare();
		renderer_render();
		renderer_present();

		frames++;

		since_last_print = current_time - last_print;

		if (since_last_print.count() > 1000.0f)
		{
			last_print = std::chrono::system_clock::now();
			std::cout << "frames: " << frames << std::endl;
			frames = 0;
		}

		current_time = std::chrono::system_clock::now();
		work_time = current_time - last_frame;

		std::chrono::duration<double, std::milli> delta_ms(delta_time - work_time.count());
		auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
		std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
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
