#pragma once
#include <vector>
#include "renderer.h"

std::vector<Renderer::RenderData> renderdata;

bool game_init();
void game_run();
void game_close();
void game_clean_up();
