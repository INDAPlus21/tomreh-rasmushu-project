#pragma once
#include <vector>
#include "renderer.h"

void initScene(Scene &scene);
bool game_init(Scene &scene);
void game_run(Scene &scene);
void game_close();
void game_clean_up();
