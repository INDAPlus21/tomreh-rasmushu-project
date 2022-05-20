#include <iostream>

#include "game.h"

int main() 
{
    Scene scene;

    if (!game_init(scene))
    {
        std::cout << "Game closed due to error" << std::endl;
        return 1;
    }

    game_run(scene);
    game_clean_up();

    return 0;
}
