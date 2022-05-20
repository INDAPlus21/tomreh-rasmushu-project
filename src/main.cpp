#include <iostream>

#include "game.h"

//#ifdef _WIN32
//extern "C" {
//    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//}
//#endif

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
