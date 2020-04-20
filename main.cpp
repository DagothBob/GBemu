/*
 * GBemu
 * - A GameBoy emulator that is not better
 *   than any other emulator at anything.
 * 
 * Made by:
 * - Holger Jensen
 * - https://github.com/DagothBob/
 * 
 * Gameboy CPU manual:
 * http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
 * 
 * The purpose of code in this class is to pass
 * data in compatible formats, between GB components
 * and real components eg. send pixel data to SDL,
 * send the ROM file to GB memory, etc.
 */

#include "main.h"

int main(int argc, char** argv) {
    //////////////////////////////
    // SDL initialization stuff //
    //////////////////////////////
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError();
    }
    initDisplay();
    
    ///////////////
    // Main loop //
    ///////////////
    quit = false;
    std::cout << (0b11111111 >> 1);
    std::cout << (0b10000000 >> 1);
    
    while (!quit)
    {
        handleEvents();
    }
    std::exit(0);
}

////////////////////////
// Clean up resources //
////////////////////////
void exit() {
    SDL_Quit();
}

////////////////////////////
// Initialize SDL display //
////////////////////////////
void initDisplay() {
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
void handleEvents() {
    SDL_Event e;

    // SDL events //
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        default:
            break;
        }
    }
}