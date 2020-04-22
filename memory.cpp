/*
* Handles the memory map as well as
* cartridge components such as MBCs, etc.
*/

#include "memory.h"

// Entire memory map (524KB address space)
//    uint8_t ROMbank0[0x4000];      0x0000 - 0x3FFF
//    uint8_t ROMbank_sw[0x4000];    0x4000 - 0x7FFF
//    uint8_t vRAM[0x2000];          0x8000 - 0x9FFF
//    uint8_t sw_RAM[0x2000];        0xA000 - 0xBFFF
//    uint8_t RAM[0x2000];           0xC000 - 0xDFFF
//    uint8_t echo_RAM[0x1E00];      0xE000 - 0xFDFF
//    uint8_t sprite_attrib[0x100];  0xFE00 - 0xFEFF
//    uint8_t IO_ports[0x80];        0xFF00 - 0xFF7F
//    uint8_t RAM2[0x80];            0xFF80 - 0xFFFF
struct Memory::memoryMap memory_map;

// Initialize memory to boot-up state
Memory::Memory() {
    fill_zeroes(memory_map);
    set_memory(0xFF10, 0x80);
    set_memory(0xFF11, 0x88);
    set_memory(0xFF12, 0xF3);
    set_memory(0xFF14, 0xBF);
    set_memory(0xFF16, 0x3F);
    set_memory(0xFF19, 0xBF);
    set_memory(0xFF1A, 0x7F);
    set_memory(0xFF1B, 0xFF);
    set_memory(0xFF1C, 0x9F);
    set_memory(0xFF1E, 0xBF);
    set_memory(0xFF20, 0xFF);
    set_memory(0xFF23, 0xBF);
    set_memory(0xFF24, 0x77);
    set_memory(0xFF25, 0xF3);
    set_memory(0xFF26, 0xF1);
    set_memory(0xFF40, 0x91);
    set_memory(0xFF47, 0xFC);
    set_memory(0xFF48, 0xFF);
    set_memory(0xFF49, 0xFF);
}

// GB is little-endian
void Memory::set_memory(uint16_t addr, uint8_t val) {
    if (addr < 0x4000) {
        memory_map.ROMbank0[addr] = val;
    }
    else if (addr < 0x8000) {
        memory_map.ROMbank_sw[addr - 0x4000] = val;
    }
    else if (addr < 0xA000) {
        memory_map.vRAM[addr - 0x8000] = val;
    }
    else if (addr < 0xC000) {
        memory_map.sw_RAM[addr - 0xA000] = val;
    }
    else if (addr < 0xE000) {
        memory_map.RAM[addr - 0xC000] = val;

        if ((addr - 0xC000) < sizeof(memory_map.echo_RAM))
            memory_map.echo_RAM[addr - 0xC000] = val;
    }
    else if (addr < 0xFE00) {
        memory_map.RAM[addr - 0xE000] = val;

        if ((addr - 0xC000) < sizeof(memory_map.echo_RAM))
            memory_map.echo_RAM[addr - 0xE000] = val;
    }
    else if (addr < 0xFF00) {
        memory_map.sprite_attrib[addr - 0xFE00] = val;
    }
    else if (addr < 0xFF80) {
        memory_map.IO_ports[addr - 0xFF00] = val;
    }
    else if (addr <= 0xFFFF) {
        memory_map.RAM2[addr - 0xFF80] = val;
    }
    else {
        std::cerr << "Error: Memory write outside address space \n";
        std::cerr << "At address " << addr << "\n";
        std::cerr << "With value " << val << "\n";
        std::exit(1);
    }
}

// GB is little-endian
uint8_t Memory::get_memory(uint16_t addr) {
    if (addr < 0x4000) {
        return memory_map.ROMbank0[addr];
    }
    else if (addr < 0x8000) {
        return memory_map.ROMbank_sw[addr - 0x4000];
    }
    else if (addr < 0xA000) {
        return memory_map.vRAM[addr - 0x8000];
    }
    else if (addr < 0xC000) {
        return memory_map.sw_RAM[addr - 0xA000];
    }
    else if (addr < 0xE000) {
        return memory_map.RAM[addr - 0xC000];
    }
    else if (addr < 0xFE00) {
        return memory_map.RAM[addr - 0xE000];
    }
    else if (addr < 0xFF00) {
        return memory_map.sprite_attrib[addr - 0xFE00];
    }
    else if (addr < 0xFF80) {
        return memory_map.IO_ports[addr - 0xFF00];
    }
    else if (addr <= 0xFFFF) {
        return memory_map.RAM2[addr - 0xFF80];
    }
    else {
        std::cerr << "Error: Memory read outside address space \n";
        std::cerr << "At address " << addr << "\n";
        std::exit(1);
    }
}

// Private ////////////////////

// For memory initialization
void Memory::fill_zeroes(Memory::memoryMap p) {
    for (std::size_t i = 0; i < sizeof(p.ROMbank0); i++) {
        p.ROMbank0[i] = 0;
    }
    for (std::size_t i = 0; i < sizeof(p.ROMbank_sw); i++) {
        p.ROMbank_sw[i] = 0;
    }
    for (std::size_t i = 0; i < sizeof(p.vRAM); i++) {
        p.vRAM[i] = 0;
    }
    for (std::size_t i = 0; i < sizeof(p.sw_RAM); i++) {
        p.sw_RAM[i] = 0;
    }
    for (std::size_t i = 0; i < sizeof(p.RAM); i++) {
        p.RAM[i] = 0;
    }
    for (std::size_t i = 0; i < sizeof(p.echo_RAM); i++) {
        p.echo_RAM[i] = 0;
    }
    for (std::size_t i = 0; i < sizeof(p.sprite_attrib); i++) {
        p.sprite_attrib[i] = 0;
    }
    for (std::size_t i = 0; i < sizeof(p.IO_ports); i++) {
        p.IO_ports[i] = 0;
    }
    for (std::size_t i = 0; i < sizeof(p.RAM2); i++) {
        p.RAM2[i] = 0;
    }
}

void Memory::init_stack(Memory::memoryMap p) {
}