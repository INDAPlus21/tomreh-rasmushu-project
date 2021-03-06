#include "game.h"

#include <thread>
#include <iostream>
#include <chrono>

static bool s_running = false;

void initScene(Scene &scene)
{
	RenderData r_data;
	Renderer::initRenderObject(r_data);
	scene.render_list.push_back(r_data);

	FractalData f_data;
	Renderer::initFractalObject(f_data);
	scene.fractal_list.push_back(f_data);
}

bool game_init(Scene &scene)
{
	std::cout << "Initializing game" << std::endl;

	if (!Renderer::init(scene))
	{
		return false;
	}

	initScene(scene);

	return true;
}

void game_run(Scene &scene)
{
	std::cout << "Running game" << std::endl;

	s_running = true;

	while (s_running)
	{
		//if (!Renderer::render(scene))
		//{
		//	s_running = false;
		//}

		if (!Renderer::renderFractal(scene))
		{
			s_running = false;
		}
		
		Renderer::renderer_present();

		// HACK: Delta time should be calculated correctly
		std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(16));
	}
}

void game_close()
{
	std::cout << "Closing game" << std::endl;
	s_running = false;
}

void game_clean_up(Scene &scene)
{
	Renderer::renderer_clean_up(scene);
}
