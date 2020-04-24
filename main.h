#ifndef MAIN_H
#define MAIN_H

// C++ libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <ios>

// SDL libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>

// GBemu sources
#include "cpu.h"
#include "display.h"
#include "memory.h"
#include "timer.h"

bool quit;
uint64_t frameCount;
std::stringstream fpsText;
std::vector<uint8_t> rombytes;
std::ifstream rom;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* windowSurface = NULL;
SDL_Surface* pixelSurface = NULL;
SDL_Texture* pixelTexture = NULL;
SDL_Surface* fpsDisplaySurface = NULL;
SDL_Texture* fpsDisplayTexture = NULL;
TTF_Font* font = NULL;

CPU cpu;
Memory memory;
Display display;
Timer framesTimer;
Timer frameTimer;

void exit();
void loadROM(char* arg);
void initDisplay();
void handleEvents();
void handleDisplay();
void renderFPSText();
void renderGBScreen();
void syncFramerate();

#endif