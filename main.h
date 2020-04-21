#ifndef MAIN_H
#define MAIN_H

// C++ libraries
#include <iostream>

// SDL libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

// GBemu sources
#include "cpu.h"
#include "display.h"
#include "memory.h"

bool quit;

void exit();
void initDisplay();
void handleEvents();

#endif