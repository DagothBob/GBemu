// C++ libraries
#include <iostream>

// SDL libraries
#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"

int main(int argc, char** argv) {
    //////////////////////////////
    // SDL initialization stuff //
    //////////////////////////////
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError();
    }
    SDL_Window* window = SDL_CreateWindow("GBemu",
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED,
                                          640, 576, 0);
    
    ///////////////
    // Main loop //
    ///////////////
    while (true)
    {
    }
    return 0;
}