#include <iostream>

#include "game.h"

int main() { 
    if (!game_init())
    {
        std::cout << "Game closed due to error" << std::endl;
        return 1;
    }

    game_run();
    game_clean_up();

    return 0;
}
