/*
 * Handles the memory map as well as
 * cartridge components such as MBCs, etc.
 */

#include <cstdint>
#include <cstddef>
#include <iostream>

namespace emu {
    class memory {
        public:

        // Entire addressable memory
        struct memoryMap                       // 64KB addressable
        {                                      // (inclusive)
            std::uint8_t ROMbank0[0x4000];     // 0x0000 - 0x3FFF
            std::uint8_t ROMbank_sw[0x4000];   // 0x4000 - 0x7FFF
            std::uint8_t vRAM[0x2000];         // 0x8000 - 0x9FFF
            std::uint8_t sw_RAM[0x2000];       // 0xA000 - 0xBFFF
            std::uint8_t RAM[0x2000];          // 0xC000 - 0xDFFF
            std::uint8_t echo_RAM[0x1E00];     // 0xE000 - 0xFDFF
            std::uint8_t sprite_attrib[0x100]; // 0xFE00 - 0xFEFF
            std::uint8_t IO_ports[0x80];       // 0xFF00 - 0xFF7F
            std::uint8_t RAM2[0x80];           // 0xFF80 - 0xFFFF
        } memory_map;

        // Initialize memory to boot-up state
        memory() {
            fill_zeroes(memory_map);
        }

        void set_memory(uint16_t addr, uint8_t val) {
            if      (addr < 0x4000) {
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
            }
            else if (addr < 0xFE00) {
                memory_map.RAM[addr - 0xE000] = val;
            }
            else if (addr < 0xFF00) {
                memory_map.sprite_attrib[addr - 0xFE00] = val;
            }
            else if (addr < 0xFF80) {
                memory_map.IO_ports[addr - 0xFF00] = val;
            }
            else if (addr < 0x10000) {
                memory_map.RAM2[addr - 0xFF80] = val;
            }
            else {
                std::cerr << "Error: Memory write outside address space \n";
                std::cerr << "At address " << addr << "\n";
                std::cerr << "With value " << val << "\n";
                std::exit(1);
            }
        }

        uint8_t get_memory(uint16_t addr) {
            if      (addr < 0x4000) {
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
            else if (addr < 0x10000) {
                return memory_map.RAM2[addr - 0xFF80];
            }
            else {
                std::cerr << "Error: Memory read outside address space \n";
                std::cerr << "At address " << addr << "\n";
                std::exit(1);
            }
        }
        
        private: //////////////////////////////////////////////////

        // For memory initialization
        void fill_zeroes(memoryMap p) {
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

        void init_stack(memoryMap p) {
            
        }
    };
}