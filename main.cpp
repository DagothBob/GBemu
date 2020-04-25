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
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        exit();
    }

    if (TTF_Init() != 0) {
        std::cout << "Error initializing SDL_TTF: " << TTF_GetError() << std::endl;
        exit();
    }

    initDisplay();

    frameCount = 0;
    framesTimer = Timer();
    frameTimer = Timer();
    font = TTF_OpenFont("/usr/share/fonts/TTF/VeraMono.ttf", 16);
    if (font == NULL) {
        std::cout << "Error loading font: " << TTF_GetError() << std::endl;
        exit();
    }

    /////////////////////
    // Gameboy startup //
    /////////////////////
    loadROM(argv[1]);
    new (&memory) Memory();
    CPU cpu(memory);

    ///////////////
    // Main loop //
    ///////////////
    quit = false;
    framesTimer.start();
    frameTimer.start();

    while (!quit) {
        tickCount = frameTimer.getTime();
        frameTimer.restart();
        handleEvents();
        handleCPU();
        handleDisplay();
        syncFramerate();
        ++frameCount;
    }
    std::exit(0);
}

// Clean up resources
void exit() {
    TTF_CloseFont(font);
    SDL_FreeSurface(pixelSurface);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(windowSurface);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

// Load from into memory from file
void loadROM(char* arg) {
    rom.open(arg, std::ios::binary | std::ios::in);
    if (!rom.is_open()) {
        std::cout << "Error opening file \'" << arg << "\'" << std::endl;
        exit();
    }

    new (&rombytes) std::vector<uint8_t>((std::istreambuf_iterator<char>(rom)), std::istreambuf_iterator<char>());

    for (auto i = 0; i < rombytes.size(); ++i) {
        memory.set_memory(i, rombytes[i]);
    }
    rom.close();
    free (&rombytes);
}

// Initialize SDL display
void initDisplay() {
    window = SDL_CreateWindow("GBemu",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        576,
        SDL_WINDOW_INPUT_FOCUS);

    if (window == NULL) {
        std::cout << "Error initializing window: " << SDL_GetError() << std::endl;
        exit();
    }
    else {
        windowSurface = SDL_GetWindowSurface(window);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
            std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
            exit();
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        }
    }
}

// Part of main loop.
// Handles input, etc.
void handleEvents() {
    SDL_Event e;

    // SDL events //
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
        case SDL_QUIT:
            quit = true;
            break;
        default:
            break;
        }
    }
}

// OPcodes
void handleCPU() {
    auto i = cpu.swap_endian(cpu.registers.PC);
    // Prefix byte
    if (rombytes[i] == 0xDD || rombytes[i] == 0xED || rombytes[i] == 0xFD) {
        cpu.inc_16bit(cpu.registers.PC);
    }
    // CB
    if (rombytes[i] == 0xCB) {
        //cpu.opcodes[0xCB](rombytes[++i], rombytes[++i]);
    }
}

// Part of main loop
// Video stuff
void handleDisplay() {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    renderFPSText();
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
}

// Renders the FPS counter text
void renderFPSText() {
    float avgFPS = (float)tickCount;

    fpsText.str("");
    fpsText << "TPF: " << avgFPS;

    fpsDisplaySurface = TTF_RenderText_Solid(font, fpsText.str().c_str(), SDL_Color{0, 0, 0, 255});
    fpsDisplayTexture = SDL_CreateTextureFromSurface(renderer, fpsDisplaySurface);
    SDL_Rect* fpsDisplayRect;
    SDL_GetClipRect(fpsDisplaySurface, fpsDisplayRect);
    SDL_RenderCopy(renderer, fpsDisplayTexture, fpsDisplayRect, fpsDisplayRect);

    fpsDisplayRect = NULL;
}

// Take the pixel information from the gameboy
// and render it to a texture
void renderGBScreen() {
}

// Wait for the frame to complete
void syncFramerate() {
    uint32_t frameTime = frameTimer.getTime();
    uint32_t tpm = (uint32_t)round(1000.f / 60.f);

    if (frameTime < tpm) {
        SDL_Delay(tpm - frameTime);
    }
}