/*
 * Handles the memory map as well as
 * cartridge components such as MBCs, etc.
 */

#include <cstdint>
#include <cstddef>

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
    };
}