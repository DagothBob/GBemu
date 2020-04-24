#include "timer.h"

Timer::Timer() {
    startTime = 0;
    started = false;
}

void Timer::start() {
    started = true;
    startTime = SDL_GetTicks();
}

void Timer::restart() {
    start();
}

uint32_t Timer::getTime() {
    return SDL_GetTicks() - startTime;
}