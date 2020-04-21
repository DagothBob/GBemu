#ifndef MEMORY_H
#define MEMORY_H

// C++ libraries
#include <cstddef>
#include <cstdint>
#include <iostream>

class Memory {
    public:
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
    void set_memory(uint16_t addr, uint8_t val);
    uint8_t get_memory(uint16_t addr);

    private:
    void fill_zeroes(memoryMap p);
    void init_stack(memoryMap p);
};

#endif