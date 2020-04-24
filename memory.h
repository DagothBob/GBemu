#ifndef MEMORY_H
#define MEMORY_H

// C++ libraries
#include <cstddef>
#include <cstdint>
#include <iostream>

class Memory {
    public:
    // clang-format off
    const uint8_t P1   = 0xFF00; // Pad IO
    const uint8_t SB   = 0xFF01; // Serial transfer data
    const uint8_t SC   = 0xFF02; // Serial IO control
    const uint8_t DIV  = 0xFF04; // Divider
    const uint8_t TIMA = 0xFF05; // Timer counter
    const uint8_t TMA  = 0xFF06; // Timer modulo
    const uint8_t TAC  = 0xFF07; // Timer control
    const uint8_t IF   = 0xFF0F; // Interrupt flag
    const uint8_t NR10 = 0xFF10; // Sound 1 (sweep)
    const uint8_t NR11 = 0xFF11; // Sound 1 (wave pattern duty)
    const uint8_t NR12 = 0xFF12; // Sound 1 (envelope)
    const uint8_t NR13 = 0xFF13; // Sound 1 (freq low)
    const uint8_t NR14 = 0xFF14; // Sound 1 (freq high)
    const uint8_t NR21 = 0xFF16; // Sound 2 (wave pattern duty)
    const uint8_t NR22 = 0xFF17; // Sound 2 (envelope)
    const uint8_t NR23 = 0xFF18; // Sound 2 (freq low)
    const uint8_t NR24 = 0xFF19; // Sound 2 (freq high)
    const uint8_t NR30 = 0xFF1A; // Sound 3 (on/off)
    const uint8_t NR31 = 0xFF1B; // Sound 3 (length)
    const uint8_t NR32 = 0xFF1C; // Sound 3 (output level)
    const uint8_t NR33 = 0xFF1D; // Sound 3 (freq low)
    const uint8_t NR34 = 0xFF1E; // Sound 3 (freq high)
    const uint8_t NR41 = 0xFF20; // Sound 4 (length)
    const uint8_t NR42 = 0xFF21; // Sound 4 (envelope)
    const uint8_t NR43 = 0xFF22; // Sound 4 (polynomial counter)
    const uint8_t NR44 = 0xFF23; // Sound 4 (counter/consecutive)
    const uint8_t NR50 = 0xFF24; // Channel control
    const uint8_t NR51 = 0xFF25; // Sound output terminal
    const uint8_t NR52 = 0xFF26; // Sound on/off
    const uint8_t LCDC = 0xFF40; // LCD control
    const uint8_t STAT = 0xFF41; // LCDC status
    const uint8_t SCY  = 0xFF42; // Scroll Y
    const uint8_t SCX  = 0xFF43; // Scroll X
    const uint8_t LY   = 0xFF44; // LCDC Y
    const uint8_t LYC  = 0xFF45; // LCDC LY-compare
    const uint8_t DMA  = 0xFF46; // DMA transfer & start address
    const uint8_t BGP  = 0xFF47; // Background/Window palette data
    const uint8_t OBP0 = 0xFF48; // Object palette 0 data
    const uint8_t OBP1 = 0xFF49; // Object palette 1 data
    const uint8_t WY   = 0xFF4A; // Window Y position
    const uint8_t WX   = 0xFF4B; // WIndow X position
    const uint8_t IE   = 0xFFFF; // Interrupt enable
    // clang-format on

    struct memoryMap {                // (inclusive)
        uint8_t ROMbank0[0x4000];     // 0x0000 - 0x3FFF
        uint8_t ROMbank_sw[0x4000];   // 0x4000 - 0x7FFF
        uint8_t vRAM[0x2000];         // 0x8000 - 0x9FFF
        uint8_t sw_RAM[0x2000];       // 0xA000 - 0xBFFF
        uint8_t RAM[0x2000];          // 0xC000 - 0xDFFF
        uint8_t echo_RAM[0x1E00];     // 0xE000 - 0xFDFF
        uint8_t sprite_attrib[0x100]; // 0xFE00 - 0xFEFF
        uint8_t IO_ports[0x80];       // 0xFF00 - 0xFF7F
        uint8_t RAM2[0x80];           // 0xFF80 - 0xFFFF
    } memory_map;

    Memory();
    Memory operator=(Memory& mem);
    void set_memory(uint16_t addr, uint8_t val);
    uint8_t get_memory(uint16_t addr);

    private:
    void fill_zeroes(memoryMap p);
    void init_stack(memoryMap p);
};

#endif