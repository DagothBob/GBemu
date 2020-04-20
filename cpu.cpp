/*
 * Sharp LR35902
 * 8-bit @ 4.19 Mhz
 */

// C++ libraries
#include <cstdint>
#include <cstddef>
#include <functional>

// GBemu sources
#include "memory.cpp"

namespace emu {
    class CPU {
        public:

        // Flags for F register
        const uint8_t FLAG_ZERO = 0b10000000;
        const uint8_t FLAG_ADSB = 0b01000000;
        const uint8_t FLAG_HALF = 0b00100000;
        const uint8_t FLAG_CARY = 0b00010000;

        // AF BC DE HL
        struct registermap {
            uint8_t A; // Accumulator, general
            uint8_t B; // Counter, general, BC byte counter
            uint8_t C; // Counter, ports, general, BC byte counter
            uint8_t D; // General, destination address
            uint8_t E; // General, destination address
            uint8_t F; // Flags - Zero | Add/Sub | Half | Carry | 0000
            uint8_t H; // General, HL 16-bit accumulator
            uint8_t L; // General, HL 16-bit accumulator
            uint16_t SP; // Stack pointer
            uint16_t PC; // Program counter
        } registers;

        // Array mapping OP code (as index) to function for handling the OP code
        void (emu::CPU::*opcodes[0x100])(uint8_t opcode, uint16_t arg) = {
        //  x0             x1             x2             x3             x4             x5             x6             x7             x8             x9             xA             xB             xC             xD             xE             xF
            op_Nop,        op_Load,       op_Load,       op_Increment,  op_Increment,  op_Decrement,  op_Load,       op_Rotate,     op_Load,       op_Add,        op_Load,       op_Decrement,  op_Increment,  op_Decrement,  op_Load,       op_Rotate,
            op_Stop,       op_Load,       op_Load,       op_Increment,  op_Increment,  op_Decrement,  op_Load,       op_Rotate,     op_Jump,       op_Add,        op_Load,       op_Decrement,  op_Increment,  op_Decrement,  op_Load,       op_Rotate,
            op_Jump,       op_Load,       op_Load,       op_Increment,  op_Increment,  op_Decrement,  op_Load,       op_Decimal,    op_Jump,       op_Add,        op_Load,       op_Decrement,  op_Increment,  op_Decrement,  op_Load,       op_Compliment,
            op_Jump,       op_Load,       op_Load,       op_Increment,  op_Increment,  op_Decrement,  op_Load,       op_Carry,      op_Jump,       op_Add,        op_Load,       op_Decrement,  op_Increment,  op_Decrement,  op_Load,       op_Carry,
            op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,
            op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,
            op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,
            op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Halt,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,       op_Load,
            op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,        op_Add,
            op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,   op_Subtract,
            op_And,        op_And,        op_And,        op_And,        op_And,        op_And,        op_And,        op_And,        op_Xor,        op_Xor,        op_Xor,        op_Xor,        op_Xor,        op_Xor,        op_Xor,        op_Xor,
            op_Or,         op_Or,         op_Or,         op_Or,         op_Or,         op_Or,         op_Or,         op_Or,         op_Compare,    op_Compare,    op_Compare,    op_Compare,    op_Compare,    op_Compare,    op_Compare,    op_Compare,
            op_Return,     op_Pop,        op_Jump,       op_Jump,       op_Call,       op_Push,       op_Add,        op_Restart,    op_Return,     op_Return,     op_Jump,       op_CB,         op_Call,       op_Call,       op_Add,        op_Restart,
            op_Return,     op_Pop,        op_Jump,       op_Unknown,    op_Call,       op_Push,       op_Subtract,   op_Restart,    op_Return,     op_Return,     op_Jump,       op_Unknown,    op_Call,       op_Unknown,    op_Subtract,   op_Restart,
            op_Load,       op_Pop,        op_Load,       op_Unknown,    op_Unknown,    op_Push,       op_And,        op_Restart,    op_Add,        op_Jump,       op_Load,       op_Unknown,    op_Unknown,    op_Unknown,    op_Xor,        op_Restart,
            op_Load,       op_Pop,        op_Load,       op_DInterrupt, op_Unknown,    op_Push,       op_Or,         op_Restart,    op_Load,       op_Load,       op_Load,       op_EInterrupt, op_Unknown,    op_Unknown,    op_Compare,    op_Restart
        //  x0             x1             x2             x3             x4             x5             x6             x7             x8             x9             xA             xB             xC             xD             xE             xF
        };

        // Maps Z80-added "CB" OP codes
        void (emu::CPU::*CBops[0x100])(uint8_t opcode, uint16_t arg) = {
        //  x0             x1             x2             x3             x4             x5             x6             x7             x8             x9             xA             xB             xC             xD             xE             xF
            op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,
            op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,     op_Rotate,
            op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,
            op_Swap,       op_Swap,       op_Swap,       op_Swap,       op_Swap,       op_Swap,       op_Swap,       op_Swap,       op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,      op_Shift,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,
            op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit,        op_Bit
        //  x0             x1             x2             x3             x4             x5             x6             x7             x8             x9             xA             xB             xC             xD             xE             xF
        };

        // Pointer to emulated memory object
        emu::memory gbmemory;

        // Initialize registers to boot-up state
        CPU(emu::memory &mem) {
            gbmemory = mem;
            registers.A = 0x01;
            registers.B = 0x00;
            registers.C = 0x13;
            registers.D = 0x00;
            registers.E = 0xD8;
            registers.F = 0xB0;
            registers.H = 0x01;
            registers.L = 0x4D;
            registers.SP = 0xFFFE;
            registers.PC = 0x100;
        }

        uint16_t swap_endian(uint16_t bytes) {
            return (bytes << 8) | (bytes >> 8);
        }

        // For use with memory::get_memory, pass in little endian order
        uint16_t concat_regist(uint8_t most, uint8_t least) {
            return (uint16_t)((most << 8) | least);
        }

        void dec_16bit(uint8_t &most, uint8_t &least) {
            uint16_t dec = concat_regist(most, least) - 0x1;
            most = (uint8_t)(dec >> 8);
            least = (uint8_t)dec;
        }

        void inc_16bit(uint8_t &most, uint8_t &least) {
            uint16_t dec = concat_regist(most, least) + 0x1;
            most = (uint8_t)(dec >> 8);
            least = (uint8_t)dec;
        }

        // Load OP codes
        // arg is usually equivalent to nn in assembly
        void op_Load(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0x01):
                registers.B = (uint8_t)(arg >> 8);
                registers.C = (uint8_t)arg;
                break;
            case (0x02):
                gbmemory.set_memory(concat_regist(registers.C, registers.B), registers.A);
                break;
            case (0x06):
                registers.B = (uint8_t)arg;
                break;
            case (0x08):
                registers.SP = arg;
                break;
            case (0x0A):
                registers.A = gbmemory.get_memory(concat_regist(registers.C, registers.B));
                break;
            case (0x0E):
                registers.C = (uint8_t)arg;
                break;
            case (0x11):
                registers.D = (uint8_t)(arg >> 8);
                registers.E = (uint8_t)arg;
                break;
            case (0x12):
                gbmemory.set_memory(concat_regist(registers.E, registers.D), registers.A);
                break;
            case (0x16):
                registers.D = (uint8_t)arg;
                break;
            case (0x1A):
                registers.A = gbmemory.get_memory(concat_regist(registers.E, registers.D));
                break;
            case (0x1E):
                registers.E = (uint8_t)arg;
                break;
            case (0x21):
                registers.H = (uint8_t)(arg >> 8);
                registers.L = (uint8_t)arg;
                break;
            case (0x22):
                gbmemory.set_memory(concat_regist(registers.L, registers.H), registers.A);
                inc_16bit(registers.H, registers.L);
                break;
            case (0x26):
                registers.H = (uint8_t)arg;
                break;
            case (0x2A):
                registers.A = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                inc_16bit(registers.H, registers.L);
                break;
            case (0x2E):
                registers.L = (uint8_t)arg;
                break;
            case (0x31):
                registers.SP = arg;
                break;
            case (0x32):
                gbmemory.set_memory(concat_regist(registers.L, registers.H), registers.A);
                dec_16bit(registers.H, registers.L);
                break;
            case (0x36):
                gbmemory.set_memory(concat_regist(registers.L, registers.H), (uint8_t)arg);
                break;
            case (0x3A):
                registers.A = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                dec_16bit(registers.H, registers.L);
                break;
            case (0x3E):
                registers.A = (uint8_t)arg;
                break;
            case (0x40):
                registers.B = registers.B;
                break;
            case (0x41):
                registers.B = registers.C;
                break;
            case (0x42):
                registers.B = registers.D;
                break;
            case (0x43):
                registers.B = registers.E;
                break;
            case (0x44):
                registers.B = registers.H;
                break;
            case (0x45):
                registers.B = registers.L;
                break;
            case (0x46):
                registers.B = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                break;
            case (0x47):
                registers.B = registers.A;
                break;
            case (0x48):
                registers.C = registers.B;
                break;
            case (0x49):
                registers.C = registers.C;
                break;
            case (0x4A):
                registers.C = registers.D;
                break;
            case (0x4B):
                registers.C = registers.E;
                break;
            case (0x4C):
                registers.C = registers.H;
                break;
            case (0x4D):
                registers.C = registers.L;
                break;
            case (0x4E):
                registers.C = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                break;
            case (0x4F):
                registers.C = registers.A;
                break;
            case (0x50):
                registers.D = registers.B;
                break;
            case (0x51):
                registers.D = registers.C;
                break;
            case (0x52):
                registers.D = registers.D;
                break;
            case (0x53):
                registers.D = registers.E;
                break;
            case (0x54):
                registers.D = registers.H;
                break;
            case (0x55):
                registers.D = registers.L;
                break;
            case (0x56):
                registers.D = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                break;
            case (0x57):
                registers.D = registers.A;
                break;
            case (0x58):
                registers.E = registers.B;
                break;
            case (0x59):
                registers.E = registers.C;
                break;
            case (0x5A):
                registers.E = registers.D;
                break;
            case (0x5B):
                registers.E = registers.E;
                break;
            case (0x5C):
                registers.E = registers.H;
                break;
            case (0x5D):
                registers.E = registers.L;
                break;
            case (0x5E):
                registers.E = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                break;
            case (0x5F):
                registers.E = registers.A;
                break;
            case (0x60):
                registers.H = registers.B;
                break;
            case (0x61):
                registers.H = registers.C;
                break;
            case (0x62):
                registers.H = registers.D;
                break;
            case (0x63):
                registers.H = registers.E;
                break;
            case (0x64):
                registers.H = registers.H;
                break;
            case (0x65):
                registers.H = registers.L;
                break;
            case (0x66):
                registers.H = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                break;
            case (0x67):
                registers.H = registers.A;
                break;
            case (0x68):
                registers.L = registers.B;
                break;
            case (0x69):
                registers.L = registers.C;
                break;
            case (0x6A):
                registers.L = registers.D;
                break;
            case (0x6B):
                registers.L = registers.E;
                break;
            case (0x6C):
                registers.L = registers.H;
                break;
            case (0x6D):
                registers.L = registers.L;
                break;
            case (0x6E):
                registers.L = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                break;
            case (0x6F):
                registers.L = registers.A;
                break;
            case (0x70):
                gbmemory.set_memory(concat_regist(registers.L, registers.H), registers.B);
                break;
            case (0x71):
                gbmemory.set_memory(concat_regist(registers.L, registers.H), registers.C);
                break;
            case (0x72):
                gbmemory.set_memory(concat_regist(registers.L, registers.H), registers.D);
                break;
            case (0x73):
                gbmemory.set_memory(concat_regist(registers.L, registers.H), registers.E);
                break;
            case (0x74):
                gbmemory.set_memory(concat_regist(registers.L, registers.H), registers.H);
                break;
            case (0x75):
                gbmemory.set_memory(concat_regist(registers.L, registers.H), registers.L);
                break;
            case (0x77):
                gbmemory.set_memory(concat_regist(registers.L, registers.H), registers.A);
                break;
            case (0x78):
                registers.A = registers.B;
                break;
            case (0x79):
                registers.A = registers.C;
                break;
            case (0x7A):
                registers.A = registers.D;
                break;
            case (0x7B):
                registers.A = registers.E;
                break;
            case (0x7C):
                registers.A = registers.H;
                break;
            case (0x7D):
                registers.A = registers.L;
                break;
            case (0x7E):
                registers.A = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                break;
            case (0x7F):
                registers.A = registers.A;
                break;
            case (0xE0):
                registers.A = gbmemory.get_memory(swap_endian(0xFF00 + (uint8_t)arg));
                break;
            case (0xE2):
                gbmemory.set_memory(swap_endian(0xFF00 + registers.C), registers.A);
                break;
            case (0xEA):
                gbmemory.set_memory(swap_endian(arg), registers.A);
                break;
            case (0xF0):
                gbmemory.set_memory(swap_endian(0xFF00 + (uint8_t)arg), registers.A);
                break;
            case (0xF2):
                registers.A = gbmemory.get_memory(swap_endian(0xFF00 + registers.C));
                break;
            case (0xF8):
                uint16_t SPn = registers.SP + arg;
                uint8_t oldL = registers.L;

                registers.H = (uint8_t)(SPn >> 8);
                registers.L = (uint8_t)SPn;
                registers.F = registers.F &= ~FLAG_ZERO;
                registers.F = registers.F &= ~FLAG_ADSB;

                if (SPn < arg)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (oldL > registers.L)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xF9):
                registers.SP = concat_regist(registers.H, registers.L);
                break;
            case (0xFA):
                registers.A = gbmemory.get_memory(swap_endian(arg));
                break;
            default:
                break;
            }
        }

        void op_Push(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
                case (0xC5):
                    gbmemory.set_memory(registers.SP, registers.B);
                    registers.SP--;
                    gbmemory.set_memory(registers.SP, registers.C);
                    registers.SP--;
                    break;
                case (0xD5):
                    gbmemory.set_memory(registers.SP, registers.D);
                    registers.SP--;
                    gbmemory.set_memory(registers.SP, registers.E);
                    registers.SP--;
                    break;
                case (0xE5):
                    gbmemory.set_memory(registers.SP, registers.H);
                    registers.SP--;
                    gbmemory.set_memory(registers.SP, registers.L);
                    registers.SP--;
                    break;
                case (0xF5):
                    gbmemory.set_memory(registers.SP, registers.A);
                    registers.SP--;
                    gbmemory.set_memory(registers.SP, registers.F);
                    registers.SP--;
                    break;
                default:
                    break;
            }
        }

        void op_Pop(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
                case (0xC1):
                    registers.C = gbmemory.get_memory(registers.SP);
                    registers.SP++;
                    registers.B = gbmemory.get_memory(registers.SP);
                    registers.SP--;
                    break;
                case (0xD1):
                    registers.E = gbmemory.get_memory(registers.SP);
                    registers.SP++;
                    registers.D = gbmemory.get_memory(registers.SP);
                    registers.SP--;
                    break;
                case (0xE1):
                    registers.L = gbmemory.get_memory(registers.SP);
                    registers.SP++;
                    registers.H = gbmemory.get_memory(registers.SP);
                    registers.SP--;
                    break;
                case (0xF1):
                    registers.F = gbmemory.get_memory(registers.SP);
                    registers.SP++;
                    registers.A = gbmemory.get_memory(registers.SP);
                    registers.SP--;
                    break;
                default:
                    break;
            }
        }

        void op_Add(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0x09):
                uint16_t oldHL = concat_regist(registers.H, registers.L);
                uint16_t HL = oldHL;

                if (registers.F & FLAG_CARY > 0x0)
                    HL = concat_regist(registers.B, registers.C) + 0x1;
                else
                    HL = concat_regist(registers.B, registers.C);
                
                registers.H = (uint8_t)(HL >> 8);
                registers.L = (uint8_t)HL;
                
                if (HL == 0x0)
                    registers.F |= FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldHL > HL)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (registers.L < (uint8_t)oldHL)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x19):
                uint16_t oldHL = concat_regist(registers.H, registers.L);
                uint16_t HL = oldHL;

                if (registers.F & FLAG_CARY > 0x0)
                    HL = concat_regist(registers.D, registers.E) + 0x1;
                else
                    HL = concat_regist(registers.D, registers.E);
                
                registers.H = (uint8_t)(HL >> 8);
                registers.L = (uint8_t)HL;
                
                if (HL == 0x0)
                    registers.F |= FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldHL > HL)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (registers.L < (uint8_t)oldHL)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x29):
                uint16_t oldHL = concat_regist(registers.H, registers.L);
                uint16_t HL = oldHL;

                if (registers.F & FLAG_CARY > 0x0)
                    HL = concat_regist(registers.H, registers.L) + 0x1;
                else
                    HL = concat_regist(registers.H, registers.L);
                
                registers.H = (uint8_t)(HL >> 8);
                registers.L = (uint8_t)HL;
                
                if (HL == 0x0)
                    registers.F |= FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldHL > HL)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (registers.L < (uint8_t)oldHL)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x39):
                uint16_t oldHL = concat_regist(registers.H, registers.L);
                uint16_t HL = oldHL;

                if (registers.F & FLAG_CARY > 0x0)
                    HL = registers.SP + 0x1;
                else
                    HL = registers.SP;
                
                registers.H = (uint8_t)(HL >> 8);
                registers.L = (uint8_t)HL;
                
                if (HL == 0x0)
                    registers.F |= FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldHL > HL)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (registers.L < (uint8_t)oldHL)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x80):
                uint8_t oldA = registers.A;
                registers.A += registers.B;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if ((oldA & 0xF) + (registers.B & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x81):
                uint8_t oldA = registers.A;
                registers.A += registers.C;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if ((oldA & 0xF) + (registers.C & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x82):
                uint8_t oldA = registers.A;
                registers.A += registers.D;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if ((oldA & 0xF) + (registers.D & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x83):
                uint8_t oldA = registers.A;
                registers.A += registers.E;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if ((oldA & 0xF) + (registers.E & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x84):
                uint8_t oldA = registers.A;
                registers.A += registers.H;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if ((oldA & 0xF) + (registers.H & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x85):
                uint8_t oldA = registers.A;
                registers.A += registers.L;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if ((oldA & 0xF) + (registers.L & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x86):
                uint8_t oldA = registers.A;
                registers.A += gbmemory.get_memory(concat_regist(registers.L, registers.H));
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if ((oldA & 0xF) + (gbmemory.get_memory(concat_regist(registers.L, registers.H)) & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x87):
                uint8_t oldA = registers.A;
                registers.A += registers.A;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if ((oldA & 0xF) + (oldA & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x88):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A += (registers.B + 0x1);
                    carry = true;
                }
                else
                    registers.A += registers.B;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (carry) {
                    if ((oldA & 0xF) + ((registers.B + 0x1) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if ((oldA & 0xF) + ((registers.B) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x89):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A += (registers.C + 0x1);
                    carry = true;
                }
                else
                    registers.A += registers.C;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if (carry) {
                    if ((oldA & 0xF) + ((registers.C + 0x1) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if ((oldA & 0xF) + ((registers.C) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x8A):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A += (registers.D + 0x1);
                    carry = true;
                }
                else
                    registers.A += registers.D;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if (carry) {
                    if ((oldA & 0xF) + ((registers.D + 0x1) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if ((oldA & 0xF) + ((registers.D) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x8B):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A += (registers.E + 0x1);
                    carry = true;
                }
                else
                    registers.A += registers.E;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if (carry) {
                    if ((oldA & 0xF) + ((registers.E + 0x1) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if ((oldA & 0xF) + ((registers.E) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x8C):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A += (registers.H + 0x1);
                    carry = true;
                }
                else
                    registers.A += registers.H;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if (carry) {
                    if ((oldA & 0xF) + ((registers.H + 0x1) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if ((oldA & 0xF) + ((registers.H) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x8D):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A += (registers.L + 0x1);
                    carry = true;
                }
                else
                    registers.A += registers.L;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if (carry) {
                    if ((oldA & 0xF) + ((registers.L + 0x1) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if ((oldA & 0xF) + ((registers.L) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x8E):
                uint8_t oldA = registers.A;
                uint8_t get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A += (get + 0x1);
                    carry = true;
                }
                else
                    registers.A += get;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if (carry) {
                    if ((oldA & 0xF) + ((get + 0x1) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if ((oldA & 0xF) + ((get) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x8F):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A += (registers.A + 0x1);
                    carry = true;
                }
                else
                    registers.A += registers.A;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if (carry) {
                    if ((oldA & 0xF) + ((oldA + 0x1) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if ((oldA & 0xF) + ((oldA) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0xC6):
                uint8_t oldA = registers.A;
                registers.A += (uint8_t)arg;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if ((oldA & 0xF) + (((uint8_t)arg) & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xCE):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A += ((uint8_t)arg + 0x1);
                    carry = true;
                }
                else
                    registers.A += (uint8_t)arg;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA > registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;

                if (carry) {
                    if ((oldA & 0xF) + (((uint8_t)arg + 0x1) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if ((oldA & 0xF) + (((uint8_t)arg) & 0xF) & 0x10)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0xE8):
                uint16_t oldSP = registers.SP;

                if (registers.F & FLAG_CARY > 0x0)
                    registers.SP = arg + 0x1;
                else
                    registers.SP = arg;
                
                registers.F &= ~FLAG_ZERO;
                registers.F &= ~FLAG_ADSB;

                if (oldSP > registers.SP)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if ((uint8_t)registers.SP < (uint8_t)oldSP)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            default:
                break;
            }
        }

        void op_Subtract(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0x90):
                uint8_t oldA = registers.A;
                registers.A -= registers.B;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((oldA & 0xF) - (registers.B & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x91):
                uint8_t oldA = registers.A;
                registers.A -= registers.C;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((oldA & 0xF) - (registers.C & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x92):
                uint8_t oldA = registers.A;
                registers.A -= registers.D;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((oldA & 0xF) - (registers.D & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x93):
                uint8_t oldA = registers.A;
                registers.A -= registers.E;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((oldA & 0xF) - (registers.E & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x94):
                uint8_t oldA = registers.A;
                registers.A -= registers.H;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((oldA & 0xF) - (registers.H & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x95):
                uint8_t oldA = registers.A;
                registers.A -= registers.L;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((oldA & 0xF) - (registers.L & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x96):
                uint8_t oldA = registers.A;
                registers.A -= gbmemory.get_memory(concat_regist(registers.L, registers.H));
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((oldA & 0xF) - (gbmemory.get_memory(concat_regist(registers.L, registers.H)) & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x97):
                uint8_t oldA = registers.A;
                registers.A -= registers.A;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((oldA & 0xF) - (registers.A & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x98):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A -= (registers.B - 0x1);
                    carry = true;
                }
                else
                    registers.A -= registers.B;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (carry) {
                    if (((oldA & 0xF) - ((registers.B - 0x1) & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if (((oldA & 0xF) - (registers.B & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x99):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A -= (registers.C - 0x1);
                    carry = true;
                }
                else
                    registers.A -= registers.C;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (carry) {
                    if (((oldA & 0xF) - ((registers.C - 0x1) & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if (((oldA & 0xF) - (registers.C & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x9A):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A -= (registers.D - 0x1);
                    carry = true;
                }
                else
                    registers.A -= registers.D;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (carry) {
                    if (((oldA & 0xF) - ((registers.D - 0x1) & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if (((oldA & 0xF) - (registers.D & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x9B):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A -= (registers.E - 0x1);
                    carry = true;
                }
                else
                    registers.A -= registers.E;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (carry) {
                    if (((oldA & 0xF) - ((registers.E - 0x1) & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if (((oldA & 0xF) - (registers.E & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x9C):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A -= (registers.H - 0x1);
                    carry = true;
                }
                else
                    registers.A -= registers.H;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (carry) {
                    if (((oldA & 0xF) - ((registers.H - 0x1) & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if (((oldA & 0xF) - (registers.H & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x9D):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A -= (registers.L - 0x1);
                    carry = true;
                }
                else
                    registers.A -= registers.L;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (carry) {
                    if (((oldA & 0xF) - ((registers.L - 0x1) & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if (((oldA & 0xF) - (registers.L & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x9E):
                uint8_t oldA = registers.A;
                uint8_t get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A -= (get - 0x1);
                    carry = true;
                }
                else
                    registers.A -= get;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (carry) {
                    if (((oldA & 0xF) - ((get - 0x1) & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if (((oldA & 0xF) - (get & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0x9F):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A -= (registers.A - 0x1);
                    carry = true;
                }
                else
                    registers.A -= registers.A;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (carry) {
                    if (((oldA & 0xF) - ((registers.A - 0x1) & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if (((oldA & 0xF) - (registers.A & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            case (0xD6):
                uint8_t oldA = registers.A;
                registers.A -= (uint8_t)arg;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((oldA & 0xF) - ((uint8_t)arg & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xDE):
                uint8_t oldA = registers.A;
                bool carry = false;

                if (registers.F & FLAG_CARY > 0x0) {
                    registers.A -= ((uint8_t)arg - 0x1);
                    carry = true;
                }
                else
                    registers.A -= (uint8_t)arg;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (oldA < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (carry) {
                    if (((oldA & 0xF) - (((uint8_t)arg - 0x1) & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                else {
                    if (((oldA & 0xF) - ((uint8_t)arg & 0xF) & 0x10) < 0)
                        registers.F |= FLAG_HALF;
                    else
                        registers.F &= ~FLAG_HALF;
                }
                break;
            default:
                break;
            }
        }

        void op_And(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0xA0):
                registers.A &= registers.B;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F |= FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xA1):
                registers.A &= registers.C;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F |= FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xA2):
                registers.A &= registers.D;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F |= FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xA3):
                registers.A &= registers.E;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F |= FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xA4):
                registers.A &= registers.H;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F |= FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xA5):
                registers.A &= registers.L;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F |= FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xA6):
                registers.A &= gbmemory.get_memory(concat_regist(registers.L, registers.H));

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F |= FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xA7):
                registers.A &= registers.A;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F |= FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xE6):
                registers.A &= (uint8_t)arg;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F |= FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            default:
                break;
            }
        }

        void op_Or(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0xB0):
                registers.A |= registers.B;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xB1):
                registers.A |= registers.C;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xB2):
                registers.A |= registers.D;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xB3):
                registers.A |= registers.E;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xB4):
                registers.A |= registers.H;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xB5):
                registers.A |= registers.L;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xB6):
                registers.A |= gbmemory.get_memory(concat_regist(registers.L, registers.H));

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xB7):
                registers.A |= registers.A;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xF6):
                registers.A |= (uint8_t)arg;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            }
        }

        void op_Xor(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0xA8):
                registers.A ^= registers.B;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xA9):
                registers.A ^= registers.C;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xAA):
                registers.A ^= registers.D;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xAB):
                registers.A ^= registers.E;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xAC):
                registers.A ^= registers.H;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xAD):
                registers.A ^= registers.L;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xAE):
                registers.A ^= gbmemory.get_memory(concat_regist(registers.L, registers.H));

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xAF):
                registers.A ^= registers.A;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0xEE):
                registers.A ^= (uint8_t)arg;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            default:
                break;
            }
        }

        void op_Compare(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0xB8):
                uint8_t comp = registers.A - registers.B;
                
                if (comp == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (comp < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((registers.A & 0xF) - (registers.B & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xB9):
                uint8_t comp = registers.A - registers.C;
                
                if (comp == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (comp < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((registers.A & 0xF) - (registers.C & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xBA):
                uint8_t comp = registers.A - registers.D;
                
                if (comp == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (comp < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((registers.A & 0xF) - (registers.D & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xBB):
                uint8_t comp = registers.A - registers.E;
                
                if (comp == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (comp < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((registers.A & 0xF) - (registers.E & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xBC):
                uint8_t comp = registers.A - registers.H;
                
                if (comp == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (comp < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((registers.A & 0xF) - (registers.H & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xBD):
                uint8_t comp = registers.A - registers.L;
                
                if (comp == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (comp < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((registers.A & 0xF) - (registers.L & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xBE):
                uint8_t get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                uint8_t comp = registers.A - get;
                
                if (comp == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (comp < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((registers.A & 0xF) - (get & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xBF):
                uint8_t comp = registers.A - registers.A;
                
                if (comp == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (comp < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((registers.A & 0xF) - (registers.A & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0xFE):
                uint8_t comp = registers.A - (uint8_t)arg;
                
                if (comp == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;

                if (comp < registers.A)
                    registers.F |= FLAG_CARY;
                else
                    registers.F &= ~FLAG_CARY;
                
                if (((registers.A & 0xF) - ((uint8_t)arg & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            default:
                break;
            }
        }

        void op_Increment(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0x03):
                uint16_t old = concat_regist(registers.B, registers.C);
                uint16_t neww = old + 1;
                registers.B = (uint8_t)(neww >> 8);
                registers.C = (uint8_t)neww;
                break;
            case (0x04):
                uint8_t old = registers.B;
                registers.B++;
                
                if (registers.B == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;
                
                if ((old & 0xF) + (0x1 & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x0C):
                uint8_t old = registers.C;
                registers.C++;
                
                if (registers.C == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;
                
                if ((old & 0xF) + (0x1 & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x13):
                uint16_t old = concat_regist(registers.D, registers.E);
                uint16_t neww = old + 1;
                registers.D = (uint8_t)(neww >> 8);
                registers.E = (uint8_t)neww;
                break;
            case (0x14):
                uint8_t old = registers.D;
                registers.D++;
                
                if (registers.D == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;
                
                if ((old & 0xF) + (0x1 & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x1C):
                uint8_t old = registers.E;
                registers.E++;
                
                if (registers.E == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;
                
                if ((old & 0xF) + (0x1 & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x23):
                uint16_t old = concat_regist(registers.H, registers.L);
                uint16_t neww = old + 1;
                registers.H = (uint8_t)(neww >> 8);
                registers.L = (uint8_t)neww;
                break;
            case (0x24):
                uint8_t old = registers.H;
                registers.H++;
                
                if (registers.H == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;
                
                if ((old & 0xF) + (0x1 & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x2C):
                uint8_t old = registers.L;
                registers.L++;
                
                if (registers.L == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;
                
                if ((old & 0xF) + (0x1 & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x33):
                registers.SP++;
                break;
            case (0x34):
                uint8_t old = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                gbmemory.set_memory(concat_regist(registers.L, registers.H), old + 1);
                uint8_t neww = old + 1;
                
                if (neww == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;
                
                if ((old & 0xF) + (0x1 & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x3C):
                uint8_t old = registers.A;
                registers.A++;
                
                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F &= ~FLAG_ADSB;
                
                if ((old & 0xF) + (0x1 & 0xF) & 0x10)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            default:
                break;
            }
        }

        void op_Decrement(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0x05):
                uint8_t old = registers.B;
                registers.B--;
                
                if (registers.B == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;
                
                if (((old & 0xF) - (0x1 & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x0B):
                uint16_t old = concat_regist(registers.B, registers.C);
                uint16_t neww = old - 1;
                registers.B = (uint8_t)(neww >> 8);
                registers.C = (uint8_t)neww;
                break;
            case (0x0D):
                uint8_t old = registers.C;
                registers.C--;
                
                if (registers.C == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;
                
                if (((old & 0xF) - (0x1 & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x15):
                uint8_t old = registers.D;
                registers.D--;
                
                if (registers.D == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;
                
                if (((old & 0xF) - (0x1 & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x1B):
                uint16_t old = concat_regist(registers.D, registers.E);
                uint16_t neww = old - 1;
                registers.D = (uint8_t)(neww >> 8);
                registers.E = (uint8_t)neww;
                break;
            case (0x1D):
                uint8_t old = registers.E;
                registers.E--;
                
                if (registers.E == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;
                
                if (((old & 0xF) - (0x1 & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x25):
                uint8_t old = registers.H;
                registers.H--;
                
                if (registers.H == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;
                
                if (((old & 0xF) - (0x1 & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x2B):
                uint16_t old = concat_regist(registers.H, registers.L);
                uint16_t neww = old - 1;
                registers.H = (uint8_t)(neww >> 8);
                registers.L = (uint8_t)neww;
                break;
            case (0x2D):
                uint8_t old = registers.L;
                registers.L--;
                
                if (registers.L == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;
                
                if (((old & 0xF) - (0x1 & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x35):
                uint8_t old = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                gbmemory.set_memory(concat_regist(registers.L, registers.H), old + 1);
                uint8_t neww = old - 1;
                
                if (neww == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;

                registers.F |= FLAG_ADSB;
                
                if (((old & 0xF) - (0x1 & 0xF) & 0x10) < 0)
                    registers.F |= FLAG_HALF;
                else
                    registers.F &= ~FLAG_HALF;
                break;
            case (0x3B):
                registers.SP--;
                break;
            default:
                break;
            }
        }

        void op_Swap(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0x30):
                registers.B = (registers.B << 4) | (registers.B >> 4);

                if (registers.B == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0x31):
                registers.C = (registers.C << 4) | (registers.C >> 4);

                if (registers.C == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0x32):
                registers.D = (registers.D << 4) | (registers.D >> 4);

                if (registers.D == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0x33):
                registers.E = (registers.E << 4) | (registers.E >> 4);

                if (registers.E == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0x34):
                registers.H = (registers.H << 4) | (registers.H >> 4);

                if (registers.H == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0x35):
                registers.L = (registers.L << 4) | (registers.L >> 4);

                if (registers.L == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0x36):
                uint8_t old = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                uint8_t neww = (old << 4) | (old >> 4);
                gbmemory.set_memory(concat_regist(registers.L, registers.H), neww);

                if (neww == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            case (0x37):
                registers.A = (registers.A << 4) | (registers.A >> 4);

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else
                    registers.F &= ~FLAG_ZERO;
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;
                registers.F &= ~FLAG_CARY;
                break;
            default:
                break;
            }
        }

        void op_Decimal(uint8_t opcode, uint16_t arg) {
            uint8_t old = registers.A;
            uint8_t nibup = (registers.A >> 4) * 10;
            uint8_t nibdn = (registers.A << 4) >> 4;

            registers.A = nibup + nibdn;

            if (registers.A == 0x0)
                registers.F |= FLAG_ZERO;
            else
                registers.F &= ~FLAG_ZERO;
            
            registers.F &= ~FLAG_HALF;

            if (old > registers.A)
                registers.F |= FLAG_CARY;
            else 
                registers.F &= ~FLAG_CARY;
        }

        void op_Compliment(uint8_t opcode, uint16_t arg) {
            registers.A = ~registers.A;
            registers.F |= FLAG_ADSB;
            registers.F |= FLAG_HALF;
        }

        void op_CompCarry(uint8_t opcode, uint16_t arg) {
            if (registers.F & FLAG_CARY == 0x0)
                registers.F |= FLAG_CARY;
            else 
                registers.F &= ~FLAG_CARY;

            registers.F &= ~FLAG_ADSB;
            registers.F &= ~FLAG_HALF;
        }

        void op_Carry(uint8_t opcode, uint16_t arg) {
            registers.F |= FLAG_CARY;
            registers.F &= ~FLAG_ADSB;
            registers.F &= ~FLAG_HALF;
        }

        void op_Nop(uint8_t opcode, uint16_t arg) {
            //Op? Nop. 
        }

        void op_Halt(uint8_t opcode, uint16_t arg) {
            // TODO
        }

        void op_Stop(uint8_t opcode, uint16_t arg) {
            // TODO
        }

        void op_DInterrupt(uint8_t opcode, uint16_t arg) {
            // TODO
        }

        void op_EInterrupt(uint8_t opcode, uint16_t arg) {
            // TODO
        }

        void op_Rotate(uint8_t opcode, uint16_t arg) {
            switch (opcode) {
            case (0x00):
                if ((registers.B >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.B = (registers.B << 1) | (registers.B >> 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.B == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x01):
                if ((registers.C >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.C = (registers.C << 1) | (registers.C >> 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.C == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x02):
                if ((registers.D >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.D = (registers.D << 1) | (registers.D >> 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.D == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x03):
                if ((registers.E >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.E = (registers.E << 1) | (registers.E >> 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.E == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x04):
                if ((registers.H >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.H = (registers.H << 1) | (registers.H >> 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.H == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x05):
                if ((registers.L >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.L = (registers.L << 1) | (registers.L >> 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.L == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x06):
                uint8_t get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                if ((get >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                uint8_t neww = (get << 1) | (get >> 7);
                gbmemory.set_memory(concat_regist(registers.L, registers.H), neww);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (neww == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x07):
                if ((registers.A >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.A = (registers.A << 1) | (registers.A >> 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x08):
                if ((registers.B << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.B = (registers.B >> 1) | (registers.B << 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.B == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x09):
                if ((registers.C << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.C = (registers.C >> 1) | (registers.C << 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.C == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x0A):
                if ((registers.D << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.D = (registers.D >> 1) | (registers.D << 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.D == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x0B):
                if ((registers.E << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.E = (registers.E >> 1) | (registers.E << 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.E == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x0C):
                if ((registers.H << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.H = (registers.H >> 1) | (registers.H << 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.H == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x0D):
                if ((registers.L << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.L = (registers.L >> 1) | (registers.L << 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.L == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x0E):
                uint8_t get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                if ((get << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                uint8_t neww = (get >> 1) | (get << 7);
                gbmemory.set_memory(concat_regist(registers.L, registers.H), neww);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (neww == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x0F):
                if ((registers.A << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                registers.A = (registers.A >> 1) | (registers.A << 7);
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x10):
                if ((registers.B >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.B = (registers.B << 1) | (registers.B >> 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.B == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x11):
                if ((registers.C >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.C = (registers.C << 1) | (registers.C >> 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.C == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x12):
                if ((registers.D >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.D = (registers.D << 1) | (registers.D >> 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.D == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x13):
                if ((registers.E >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.E = (registers.E << 1) | (registers.E >> 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.E == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x14):
                if ((registers.H >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.H = (registers.H << 1) | (registers.H >> 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.H == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x15):
                if ((registers.L >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.L = (registers.L << 1) | (registers.L >> 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.L == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x16):
                uint8_t get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                if ((get >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                uint8_t set = (get << 1) | (get >> 7);
                if (registers.F & FLAG_CARY) 
                    gbmemory.set_memory(concat_regist(registers.L, registers.H), set);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if ((set == 0x0 && registers.F & FLAG_CARY) || (get == 0x0 && ~(registers.F & FLAG_CARY)))
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x17):
                if ((registers.A >> 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.A = (registers.A << 1) | (registers.A >> 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x18):
                if ((registers.B << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.B = (registers.B >> 1) | (registers.B << 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.B == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x19):
                if ((registers.C << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.C = (registers.C >> 1) | (registers.C << 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.C == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x1A):
                if ((registers.D << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.D = (registers.D >> 1) | (registers.D << 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.D == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x1B):
                if ((registers.E << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.E = (registers.E >> 1) | (registers.E << 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.E == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x1C):
                if ((registers.B << 7) == 0x0)
                    registers.H &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.H = (registers.H >> 1) | (registers.H << 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.H == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x1D):
                if ((registers.L << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.L = (registers.L >> 1) | (registers.L << 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.L == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x1E):
                uint8_t get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
                if ((get << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                uint8_t set = (get >> 1) | (get << 7);
                if (registers.F & FLAG_CARY) 
                    gbmemory.set_memory(concat_regist(registers.L, registers.H), set);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if ((set == 0x0 && registers.F & FLAG_CARY) || (get == 0x0 && ~(registers.F & FLAG_CARY)))
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            case (0x1F):
                if ((registers.A << 7) == 0x0)
                    registers.F &= ~FLAG_CARY;
                else 
                    registers.F |= FLAG_CARY;
                
                if (registers.F & FLAG_CARY)
                    registers.A = (registers.A >> 1) | (registers.A << 7);
                
                registers.F &= ~FLAG_ADSB;
                registers.F &= ~FLAG_HALF;

                if (registers.A == 0x0)
                    registers.F |= FLAG_ZERO;
                else 
                    registers.F &= ~FLAG_ZERO;
                break;
            default:
                break;
            }
        }

        void op_Shift(uint8_t opcode, uint16_t arg) {

        }

        void op_Bit(uint8_t opcode, uint16_t arg) {

        }

        void op_Jump(uint8_t opcode, uint16_t arg) {

        }

        void op_Call(uint8_t opcode, uint16_t arg) {

        }

        void op_Restart(uint8_t opcode, uint16_t arg) {

        }

        void op_Return(uint8_t opcode, uint16_t arg) {

        }

        void op_CB(uint8_t opcode, uint16_t arg) {

        }

        void op_Unknown(uint8_t opcode, uint16_t arg) {

        }
    };
}