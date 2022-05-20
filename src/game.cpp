#include "game.h"

#include <thread>
#include <iostream>
#include <chrono>

static bool s_running = false;

void initScene(Scene scene)
{
	RenderData obj;
	Renderer::addRenderObject(obj);
	scene.render_list.push_back(obj);
}

bool game_init(Scene scene)
{
	std::cout << "Initializing game" << std::endl;

	if (!Renderer::renderer_init(scene))
	{
		return false;
	}

	initScene(scene);

	return true;
}

void game_run(Scene scene)
{
	std::cout << "Running game" << std::endl;

	s_running = true;

	while (s_running)
	{
		Renderer::renderer_prepare();

		for (RenderData data : scene.render_list)
		{
			Renderer::renderObject(data);
		}

		Renderer::drawToScreen(scene);
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

void game_clean_up()
{
	Renderer::renderer_clean_up();
}
