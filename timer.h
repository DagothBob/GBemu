#ifndef TIMER_H
#define TIMER_H

#include <cstdint>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

class Timer {
    public:
    uint32_t startTime;
    bool started;

    Timer();
    void start();
    void restart();
    uint32_t getTime();
};

#endif