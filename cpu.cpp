/*
 * Sharp LR35902
 * 8-bit @ 4.19 Mhz
 */
#include <cstdint>
#include <cstddef>

namespace emu {
    class CPU {
        public:

        struct registermap {
            uint16_t AF; // F is accumulator
            uint16_t BC;
            uint16_t DE;
            uint16_t HL;
            uint16_t SP; // Stack pointer
            uint16_t PC; // Program counter
            uint8_t Flag; // Zero | Sub | Half | Carry | 0000
        } registers;

        // Initialize registers to boot-up state
        CPU() {
            registers.SP = 0xFFFE;
            registers.PC = 0x100;
        }

        // Load OP codes
        void LD(uint8_t opcode) {

        }
    };
}