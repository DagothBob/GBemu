// C++ libraries
#include <iostream>

// SDL libraries
#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"

namespace emu {
    void initDisplay();
    void handleEvents();

    bool quit;
}

int main(int argc, char** argv) {
    //////////////////////////////
    // SDL initialization stuff //
    //////////////////////////////
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError();
    }
    emu::initDisplay();
    
    ///////////////
    // Main loop //
    ///////////////
    emu::quit = false;

    while (!emu::quit)
    {
        emu::handleEvents();
    }
    return 0;
}

////////////////////////////
// Initialize SDL display //
////////////////////////////
void emu::initDisplay() {
    SDL_Window* window = SDL_CreateWindow("GBemu",
                                        SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED,
                                        640, 576, 0);
    SDL_Surface* windowSurface = NULL;

    if (window == NULL) {
        std::cout << "Error initializing window: " << SDL_GetError();
    }
    else {
        windowSurface = SDL_GetWindowSurface(window);
    }
}

/////////////////////////
// Part of main loop.  //
// Handles input, etc. //
/////////////////////////
void emu::handleEvents() {
    SDL_Event e;

    // SDL events //
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type)
        {
        case SDL_QUIT:
            emu::quit = true;
            break;
        default:
            break;
        }
    }
}