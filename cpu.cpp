/*
* Sharp LR35902
* 8-bit @ 4.19 Mhz
*/

#include "cpu.h"

// AF BC DE HL
struct CPU::registerMap registers;

// Array mapping OP code (as index) to function for handling the OP code
void (CPU::*opcodes[0x100])(uint8_t, uint16_t) = {
    // clang-format off
//  x0                  x1                  x2                  x3                  x4                  x5                  x6                  x7                  x8                  x9                  xA                  xB                  xC                  xD                  xE                  xF
    &CPU::op_Nop,       &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Increment, &CPU::op_Increment, &CPU::op_Decrement, &CPU::op_Load,      &CPU::op_Rotate,    &CPU::op_Load,      &CPU::op_Add,       &CPU::op_Load,      &CPU::op_Decrement, &CPU::op_Increment, &CPU::op_Decrement, &CPU::op_Load,      &CPU::op_Rotate,
    &CPU::op_Stop,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Increment, &CPU::op_Increment, &CPU::op_Decrement, &CPU::op_Load,      &CPU::op_Rotate,    &CPU::op_Jump,      &CPU::op_Add,       &CPU::op_Load,      &CPU::op_Decrement, &CPU::op_Increment, &CPU::op_Decrement, &CPU::op_Load,      &CPU::op_Rotate,
    &CPU::op_Jump,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Increment, &CPU::op_Increment, &CPU::op_Decrement, &CPU::op_Load,      &CPU::op_Decimal,   &CPU::op_Jump,      &CPU::op_Add,       &CPU::op_Load,      &CPU::op_Decrement, &CPU::op_Increment, &CPU::op_Decrement, &CPU::op_Load,      &CPU::op_Complement,
    &CPU::op_Jump,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Increment, &CPU::op_Increment, &CPU::op_Decrement, &CPU::op_Load,      &CPU::op_Carry,     &CPU::op_Jump,      &CPU::op_Add,       &CPU::op_Load,      &CPU::op_Decrement, &CPU::op_Increment, &CPU::op_Decrement, &CPU::op_Load,      &CPU::op_Carry,
    &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,
    &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,
    &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,
    &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Halt,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,
    &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,       &CPU::op_Add,
    &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,  &CPU::op_Subtract,
    &CPU::op_And,       &CPU::op_And,       &CPU::op_And,       &CPU::op_And,       &CPU::op_And,       &CPU::op_And,       &CPU::op_And,       &CPU::op_And,       &CPU::op_Xor,       &CPU::op_Xor,       &CPU::op_Xor,       &CPU::op_Xor,       &CPU::op_Xor,       &CPU::op_Xor,       &CPU::op_Xor,       &CPU::op_Xor,
    &CPU::op_Or,        &CPU::op_Or,        &CPU::op_Or,        &CPU::op_Or,        &CPU::op_Or,        &CPU::op_Or,        &CPU::op_Or,        &CPU::op_Or,        &CPU::op_Compare,   &CPU::op_Compare,   &CPU::op_Compare,   &CPU::op_Compare,   &CPU::op_Compare,   &CPU::op_Compare,   &CPU::op_Compare,   &CPU::op_Compare,
    &CPU::op_Return,    &CPU::op_Pop,       &CPU::op_Jump,      &CPU::op_Jump,      &CPU::op_Call,      &CPU::op_Push,      &CPU::op_Add,       &CPU::op_Restart,   &CPU::op_Return,    &CPU::op_Return,    &CPU::op_Jump,      &CPU::op_CB,        &CPU::op_Call,      &CPU::op_Call,      &CPU::op_Add,       &CPU::op_Restart,
    &CPU::op_Return,    &CPU::op_Pop,       &CPU::op_Jump,      &CPU::op_Unknown,   &CPU::op_Call,      &CPU::op_Push,      &CPU::op_Subtract,  &CPU::op_Restart,   &CPU::op_Return,    &CPU::op_Return,    &CPU::op_Jump,      &CPU::op_Unknown,   &CPU::op_Call,      &CPU::op_Unknown,   &CPU::op_Subtract,  &CPU::op_Restart,
    &CPU::op_Load,      &CPU::op_Pop,       &CPU::op_Load,      &CPU::op_Unknown,   &CPU::op_Unknown,   &CPU::op_Push,      &CPU::op_And,       &CPU::op_Restart,   &CPU::op_Add,       &CPU::op_Jump,      &CPU::op_Load,      &CPU::op_Unknown,   &CPU::op_Unknown,   &CPU::op_Unknown,   &CPU::op_Xor,       &CPU::op_Restart,
    &CPU::op_Load,      &CPU::op_Pop,       &CPU::op_Load,      &CPU::op_DInterrupt,&CPU::op_Unknown,   &CPU::op_Push,      &CPU::op_Or,        &CPU::op_Restart,   &CPU::op_Load,      &CPU::op_Load,      &CPU::op_Load,      &CPU::op_EInterrupt,&CPU::op_Unknown,   &CPU::op_Unknown,   &CPU::op_Compare,   &CPU::op_Restart
//  x0                  x1                  x2                  x3                  x4                  x5                  x6                  x7                  x8                  x9                  xA                  xB                  xC                  xD                  xE                  xF
};

// Maps Z80-added "CB" OP codes
void (CPU::*CBops[0x100])(uint8_t, uint16_t) = {
//  x0                  x1                  x2                  x3                  x4                  x5                  x6                  x7                  x8                  x9                  xA                  xB                  xC                  xD                  xE                  xF
    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,
    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,    &CPU::op_Rotate,
    &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,
    &CPU::op_Swap,      &CPU::op_Swap,      &CPU::op_Swap,      &CPU::op_Swap,      &CPU::op_Swap,      &CPU::op_Swap,      &CPU::op_Swap,      &CPU::op_Swap,      &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,     &CPU::op_Shift,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,
    &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit,       &CPU::op_Bit
//  x0                  x1                  x2                  x3                  x4                  x5                  x6                  x7                  x8                  x9                  xA                  xB                  xC                  xD                  xE                  xF
    // clang-format on
};

CPU::CPU() {}

// Initialize registers to boot-up state
CPU::CPU(Memory& mem) {
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

uint16_t CPU::swap_endian(uint16_t bytes) {
    return (bytes << 8) | (bytes >> 8);
}

// For use with memory::get_memory, pass in little endian order
uint16_t CPU::concat_regist(uint8_t most, uint8_t least) {
    return (uint16_t)((most << 8) | least);
}

// Decrementing 8-bit combined register
void CPU::dec_16bit(uint8_t& most, uint8_t& least) {
    uint16_t dec = concat_regist(most, least) - 0x1;
    most = (uint8_t)(dec >> 8);
    least = (uint8_t)dec;
}

// Decrementing true 16-bit register
void CPU::dec_16bit(uint16_t& reg) {
    reg = swap_endian(reg - 1);
}

// Incrementing 8-bit combined register
void CPU::inc_16bit(uint8_t& most, uint8_t& least) {
    uint16_t dec = concat_regist(most, least) + 0x1;
    most = (uint8_t)(dec >> 8);
    least = (uint8_t)dec;
}

// Incrementing true 16-bit register
void CPU::inc_16bit(uint16_t& reg) {
    reg = swap_endian(reg + 1);
}

///////////////////////
// OP code functions //
///////////////////////
void CPU::op_Load(uint8_t opcode, uint16_t arg) {
    uint8_t oldL;
    uint16_t SPn;

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
        SPn = registers.SP + arg;
        oldL = registers.L;

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

void CPU::op_Push(uint8_t opcode, uint16_t arg) {
    switch (opcode) {
    case (0xC5):
        dec_16bit(registers.SP);
        gbmemory.set_memory(registers.SP, registers.B);
        dec_16bit(registers.SP);
        gbmemory.set_memory(registers.SP, registers.C);
        break;
    case (0xD5):
        dec_16bit(registers.SP);
        gbmemory.set_memory(registers.SP, registers.D);
        dec_16bit(registers.SP);
        gbmemory.set_memory(registers.SP, registers.E);
        break;
    case (0xE5):
        dec_16bit(registers.SP);
        gbmemory.set_memory(registers.SP, registers.H);
        dec_16bit(registers.SP);
        gbmemory.set_memory(registers.SP, registers.L);
        break;
    case (0xF5):
        dec_16bit(registers.SP);
        gbmemory.set_memory(registers.SP, registers.A);
        dec_16bit(registers.SP);
        gbmemory.set_memory(registers.SP, registers.F);
        break;
    default:
        break;
    }
}

void CPU::op_Pop(uint8_t opcode, uint16_t arg) {
    switch (opcode) {
    case (0xC1):
        registers.C = gbmemory.get_memory(registers.SP);
        inc_16bit(registers.SP);
        registers.B = gbmemory.get_memory(registers.SP);
        inc_16bit(registers.SP);
        break;
    case (0xD1):
        registers.E = gbmemory.get_memory(registers.SP);
        inc_16bit(registers.SP);
        registers.D = gbmemory.get_memory(registers.SP);
        inc_16bit(registers.SP);
        break;
    case (0xE1):
        registers.L = gbmemory.get_memory(registers.SP);
        inc_16bit(registers.SP);
        registers.H = gbmemory.get_memory(registers.SP);
        inc_16bit(registers.SP);
        break;
    case (0xF1):
        registers.F = gbmemory.get_memory(registers.SP);
        inc_16bit(registers.SP);
        registers.A = gbmemory.get_memory(registers.SP);
        inc_16bit(registers.SP);
        break;
    default:
        break;
    }
}

void CPU::op_Add(uint8_t opcode, uint16_t arg) {
    uint8_t oldA, get;
    uint16_t oldHL, HL, oldSP;
    bool carry;

    switch (opcode) {
    case (0x09):
        oldHL = concat_regist(registers.H, registers.L);
        HL = oldHL;

        if ((registers.F & FLAG_CARY) > 0x0)
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
        oldHL = concat_regist(registers.H, registers.L);
        HL = oldHL;

        if ((registers.F & FLAG_CARY) > 0x0)
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
        oldHL = concat_regist(registers.H, registers.L);
        HL = oldHL;

        if ((registers.F & FLAG_CARY) > 0x0)
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
        oldHL = concat_regist(registers.H, registers.L);
        HL = oldHL;

        if ((registers.F & FLAG_CARY) > 0x0)
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
            if ((oldA & 0xF) + ((get)&0xF) & 0x10)
                registers.F |= FLAG_HALF;
            else
                registers.F &= ~FLAG_HALF;
        }
        break;
    case (0x8F):
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
            if ((oldA & 0xF) + ((oldA)&0xF) & 0x10)
                registers.F |= FLAG_HALF;
            else
                registers.F &= ~FLAG_HALF;
        }
        break;
    case (0xC6):
        oldA = registers.A;
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldSP = registers.SP;

        if ((registers.F & FLAG_CARY) > 0x0)
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

void CPU::op_Subtract(uint8_t opcode, uint16_t arg) {
    uint8_t oldA, get;
    bool carry;

    switch (opcode) {
    case (0x90):
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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
        oldA = registers.A;
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
        oldA = registers.A;
        carry = false;

        if ((registers.F & FLAG_CARY) > 0x0) {
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

void CPU::op_And(uint8_t opcode, uint16_t arg) {
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

void CPU::op_Or(uint8_t opcode, uint16_t arg) {
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

void CPU::op_Xor(uint8_t opcode, uint16_t arg) {
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

void CPU::op_Compare(uint8_t opcode, uint16_t arg) {
    uint8_t comp, get;

    switch (opcode) {
    case (0xB8):
        comp = registers.A - registers.B;

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
        comp = registers.A - registers.C;

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
        comp = registers.A - registers.D;

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
        comp = registers.A - registers.E;

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
        comp = registers.A - registers.H;

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
        comp = registers.A - registers.L;

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
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        comp = registers.A - get;

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
        comp = registers.A - registers.A;

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
        comp = registers.A - (uint8_t)arg;

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

void CPU::op_Increment(uint8_t opcode, uint16_t arg) {
    uint8_t old, neww;
    uint16_t old16, neww16;

    switch (opcode) {
    case (0x03):
        old16 = concat_regist(registers.B, registers.C);
        neww16 = old16 + 1;
        registers.B = (uint8_t)(neww >> 8);
        registers.C = (uint8_t)neww;
        break;
    case (0x04):
        old = registers.B;
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
        old = registers.C;
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
        old16 = concat_regist(registers.D, registers.E);
        neww16 = old16 + 1;
        registers.D = (uint8_t)(neww >> 8);
        registers.E = (uint8_t)neww;
        break;
    case (0x14):
        old = registers.D;
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
        old = registers.E;
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
        old16 = concat_regist(registers.H, registers.L);
        neww16 = old16 + 1;
        registers.H = (uint8_t)(neww >> 8);
        registers.L = (uint8_t)neww;
        break;
    case (0x24):
        old = registers.H;
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
        old = registers.L;
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
        inc_16bit(registers.SP);
        break;
    case (0x34):
        old = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        gbmemory.set_memory(concat_regist(registers.L, registers.H), old + 1);
        neww = old + 1;

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
        old = registers.A;
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

void CPU::op_Decrement(uint8_t opcode, uint16_t arg) {
    uint8_t old, neww;
    uint16_t old16, neww16;

    switch (opcode) {
    case (0x05):
        old = registers.B;
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
        old16 = concat_regist(registers.B, registers.C);
        neww16 = old16 - 1;
        registers.B = (uint8_t)(neww >> 8);
        registers.C = (uint8_t)neww;
        break;
    case (0x0D):
        old = registers.C;
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
        old = registers.D;
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
        old16 = concat_regist(registers.D, registers.E);
        neww16 = old16 - 1;
        registers.D = (uint8_t)(neww >> 8);
        registers.E = (uint8_t)neww;
        break;
    case (0x1D):
        old = registers.E;
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
        old = registers.H;
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
        old16 = concat_regist(registers.H, registers.L);
        neww16 = old16 - 1;
        registers.H = (uint8_t)(neww >> 8);
        registers.L = (uint8_t)neww;
        break;
    case (0x2D):
        old = registers.L;
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
        old = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        gbmemory.set_memory(concat_regist(registers.L, registers.H), old + 1);
        neww = old - 1;

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
        dec_16bit(registers.SP);
        break;
    default:
        break;
    }
}

void CPU::op_Swap(uint8_t opcode, uint16_t arg) {
    uint8_t old, neww;

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
        old = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        neww = (old << 4) | (old >> 4);
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

void CPU::op_Decimal(uint8_t opcode, uint16_t arg) {
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

void CPU::op_Complement(uint8_t opcode, uint16_t arg) {
    registers.A = ~registers.A;
    registers.F |= FLAG_ADSB;
    registers.F |= FLAG_HALF;
}

void CPU::op_CompCarry(uint8_t opcode, uint16_t arg) {
    if ((registers.F & FLAG_CARY) == 0x0)
        registers.F |= FLAG_CARY;
    else
        registers.F &= ~FLAG_CARY;

    registers.F &= ~FLAG_ADSB;
    registers.F &= ~FLAG_HALF;
}

void CPU::op_Carry(uint8_t opcode, uint16_t arg) {
    registers.F |= FLAG_CARY;
    registers.F &= ~FLAG_ADSB;
    registers.F &= ~FLAG_HALF;
}

void CPU::op_Nop(uint8_t opcode, uint16_t arg) {
    //Op? Nop.
}

void CPU::op_Halt(uint8_t opcode, uint16_t arg) {
    // TODO
}

void CPU::op_Stop(uint8_t opcode, uint16_t arg) {
    // TODO
}

void CPU::op_DInterrupt(uint8_t opcode, uint16_t arg) {
    // TODO
}

void CPU::op_EInterrupt(uint8_t opcode, uint16_t arg) {
    // TODO
}

void CPU::op_Rotate(uint8_t opcode, uint16_t arg) {
    uint8_t get, neww, set;

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
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        if ((get >> 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        neww = (get << 1) | (get >> 7);
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
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        if ((get << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        neww = (get >> 1) | (get << 7);
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
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        if ((get >> 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        set = (get << 1) | (get >> 7);
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
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        if ((get << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        set = (get >> 1) | (get << 7);
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

void CPU::op_Shift(uint8_t opcode, uint16_t arg) {
    uint8_t get, set;
    bool msb;

    switch (opcode) {
    case (0x20):
        if ((registers.B >> 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.B = registers.B << 1;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.B == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x21):
        if ((registers.C >> 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.C = registers.C << 1;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.C == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x22):
        if ((registers.D >> 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.D = registers.D << 1;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.D == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x23):
        if ((registers.E >> 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.E = registers.E << 1;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.E == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x24):
        if ((registers.H >> 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.H = registers.H << 1;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.H == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x25):
        if ((registers.L >> 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.L = registers.L << 1;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.L == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x26):
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        if ((get >> 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        set = get << 1;
        if (registers.F & FLAG_CARY)
            gbmemory.set_memory(concat_regist(registers.L, registers.H), set);

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if ((set == 0x0 && registers.F & FLAG_CARY) || (get == 0x0 && ~(registers.F & FLAG_CARY)))
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x27):
        if ((registers.A >> 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.A = registers.A << 1;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.A == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x28):
        if ((registers.B << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if ((registers.B >> 7) == 0x0) {
            if (registers.F & FLAG_CARY)
                registers.B = (registers.B >> 1) | 0b00000000;
        }
        else {
            if (registers.F & FLAG_CARY)
                registers.B = (registers.B >> 1) | 0b10000000;
        }

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.B == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x29):
        if ((registers.C << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if ((registers.C >> 7) == 0x0) {
            if (registers.F & FLAG_CARY)
                registers.C = (registers.C >> 1) | 0b00000000;
        }
        else {
            if (registers.F & FLAG_CARY)
                registers.C = (registers.C >> 1) | 0b10000000;
        }

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.C == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x2A):
        if ((registers.D << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if ((registers.D >> 7) == 0x0) {
            if (registers.F & FLAG_CARY)
                registers.D = (registers.D >> 1) | 0b00000000;
        }
        else {
            if (registers.F & FLAG_CARY)
                registers.D = (registers.D >> 1) | 0b10000000;
        }

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.D == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x2B):
        if ((registers.E << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if ((registers.E >> 7) == 0x0) {
            if (registers.F & FLAG_CARY)
                registers.E = (registers.E >> 1) | 0b00000000;
        }
        else {
            if (registers.F & FLAG_CARY)
                registers.E = (registers.E >> 1) | 0b10000000;
        }

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.E == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x2C):
        if ((registers.H << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if ((registers.H >> 7) == 0x0) {
            if (registers.F & FLAG_CARY)
                registers.H = (registers.H >> 1) | 0b00000000;
        }
        else {
            if (registers.F & FLAG_CARY)
                registers.H = (registers.H >> 1) | 0b10000000;
        }

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.H == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x2D):
        if ((registers.L << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if ((registers.L >> 7) == 0x0) {
            if (registers.F & FLAG_CARY)
                registers.L = (registers.L >> 1) | 0b00000000;
        }
        else {
            if (registers.F & FLAG_CARY)
                registers.L = (registers.L >> 1) | 0b10000000;
        }

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.L == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x2E):
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));

        if ((get << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if ((get >> 7) == 0x0) {
            if (registers.F & FLAG_CARY)
                gbmemory.set_memory(concat_regist(registers.L, registers.H), (get >> 1) | 0b00000000);
        }
        else {
            if (registers.F & FLAG_CARY)
                gbmemory.set_memory(concat_regist(registers.L, registers.H), (get >> 1) | 0b10000000);
        }

        set = gbmemory.get_memory(concat_regist(registers.L, registers.H));

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (set == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x2F):
        if ((registers.A << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if ((registers.A >> 7) == 0x0) {
            if (registers.F & FLAG_CARY)
                registers.A = (registers.A >> 1) | 0b00000000;
        }
        else {
            if (registers.F & FLAG_CARY)
                registers.A = (registers.A >> 1) | 0b10000000;
        }

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.A == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x38):
        if ((registers.B << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.B = (registers.B >> 1) | 0b00000000;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.B == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x39):
        if ((registers.C << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.C = (registers.C >> 1) | 0b00000000;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.C == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x3A):
        if ((registers.D << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.D = (registers.D >> 1) | 0b00000000;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.D == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x3B):
        if ((registers.E << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.E = (registers.E >> 1) | 0b00000000;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.E == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x3C):
        if ((registers.H << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.H = (registers.H >> 1) | 0b00000000;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.H == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x3D):
        if ((registers.L << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.L = (registers.L >> 1) | 0b00000000;

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (registers.L == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x3E):
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));

        if ((get << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            gbmemory.set_memory(concat_regist(registers.L, registers.H), (get >> 1) | 0b00000000);

        set = gbmemory.get_memory(concat_regist(registers.L, registers.H));

        registers.F &= ~FLAG_ADSB;
        registers.F &= ~FLAG_HALF;

        if (set == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;
        break;
    case (0x3F):
        if ((registers.A << 7) == 0x0)
            registers.F &= ~FLAG_CARY;
        else
            registers.F |= FLAG_CARY;

        if (registers.F & FLAG_CARY)
            registers.A = (registers.A >> 1) | 0b00000000;

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

void CPU::op_Bit(uint8_t opcode, uint16_t arg) {
    uint8_t test, get, set;

    switch (opcode) {
    case (0x40):
        test = 0b00000001 << arg;

        if ((registers.B & test) == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;

        registers.F &= ~FLAG_ADSB;
        registers.F |= FLAG_HALF;
        break;
    case (0x41):
        test = 0b00000001 << arg;

        if ((registers.C & test) == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;

        registers.F &= ~FLAG_ADSB;
        registers.F |= FLAG_HALF;
        break;
    case (0x42):
        test = 0b00000001 << arg;

        if ((registers.D & test) == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;

        registers.F &= ~FLAG_ADSB;
        registers.F |= FLAG_HALF;
        break;
    case (0x43):
        test = 0b00000001 << arg;

        if ((registers.E & test) == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;

        registers.F &= ~FLAG_ADSB;
        registers.F |= FLAG_HALF;
        break;
    case (0x44):
        test = 0b00000001 << arg;

        if ((registers.H & test) == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;

        registers.F &= ~FLAG_ADSB;
        registers.F |= FLAG_HALF;
        break;
    case (0x45):
        test = 0b00000001 << arg;

        if ((registers.L & test) == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;

        registers.F &= ~FLAG_ADSB;
        registers.F |= FLAG_HALF;
        break;
    case (0x46):
        test = 0b00000001 << arg;
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));

        if ((get & test) == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;

        registers.F &= ~FLAG_ADSB;
        registers.F |= FLAG_HALF;
        break;
    case (0x47):
        test = 0b00000001 << arg;

        if ((registers.A & test) == 0x0)
            registers.F |= FLAG_ZERO;
        else
            registers.F &= ~FLAG_ZERO;

        registers.F &= ~FLAG_ADSB;
        registers.F |= FLAG_HALF;
        break;
    case (0x80):
        registers.B &= ~(0b00000001 < arg);
        break;
    case (0x81):
        registers.C &= ~(0b00000001 < arg);
        break;
    case (0x82):
        registers.D &= ~(0b00000001 < arg);
        break;
    case (0x83):
        registers.E &= ~(0b00000001 < arg);
        break;
    case (0x84):
        registers.H &= ~(0b00000001 < arg);
        break;
    case (0x85):
        registers.L &= ~(0b00000001 < arg);
        break;
    case (0x86):
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        gbmemory.set_memory(concat_regist(registers.L, registers.H), get & ~(0b00000001 < arg));
        break;
    case (0x87):
        registers.L &= ~(0b00000001 < arg);
        break;
    case (0xC0):
        registers.B |= (0b00000001 < arg);
        break;
    case (0xC1):
        registers.C |= (0b00000001 < arg);
        break;
    case (0xC2):
        registers.D |= (0b00000001 < arg);
        break;
    case (0xC3):
        registers.E |= (0b00000001 < arg);
        break;
    case (0xC4):
        registers.H |= (0b00000001 < arg);
        break;
    case (0xC5):
        registers.L |= (0b00000001 < arg);
        break;
    case (0xC6):
        get = gbmemory.get_memory(concat_regist(registers.L, registers.H));
        gbmemory.set_memory(concat_regist(registers.L, registers.H), get | (0b00000001 < arg));
        break;
    case (0xC7):
        registers.L |= (0b00000001 < arg);
        break;
    default:
        break;
    }
}

void CPU::op_Jump(uint8_t opcode, uint16_t arg) {
    switch (opcode) {
    case (0x18):
        registers.PC += (uint8_t)arg;
        break;
    case (0x20):
        if ((registers.F & FLAG_ZERO) == 0x0)
            registers.PC += (uint8_t)arg;
        break;
    case (0x28):
        if ((registers.F & FLAG_ZERO) > 0x0)
            registers.PC += (uint8_t)arg;
        break;
    case (0x30):
        if ((registers.F & FLAG_CARY) == 0x0)
            registers.PC += (uint8_t)arg;
        break;
    case (0x38):
        if ((registers.F & FLAG_ZERO) > 0x0)
            registers.PC += (uint8_t)arg;
        break;
    case (0xC2):
        if ((registers.F & FLAG_ZERO) == 0x0)
            registers.PC = arg;
        break;
    case (0xC3):
        registers.PC = arg;
        break;
    case (0xCA):
        if ((registers.F & FLAG_ZERO) > 0)
            registers.PC = arg;
        break;
    case (0xD2):
        if ((registers.F & FLAG_CARY) == 0x0)
            registers.PC = arg;
        break;
    case (0xDA):
        if ((registers.F & FLAG_CARY) > 0x0)
            registers.PC = arg;
        break;
    case (0xE9):
        registers.PC = concat_regist(registers.H, registers.L);
        break;
    default:
        break;
    }
}

void CPU::op_Call(uint8_t opcode, uint16_t arg) {
    switch (opcode) {
    case (0xC4):
        if ((registers.F & FLAG_ADSB) == 0x0) {
            dec_16bit(registers.SP);
            gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
            dec_16bit(registers.SP);
            gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
            registers.PC = arg;
        }
        break;
    case (0xCC):
        if ((registers.F & FLAG_ADSB) > 0x0) {
            dec_16bit(registers.SP);
            gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
            dec_16bit(registers.SP);
            gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
            registers.PC = arg;
        }
        break;
    case (0xCD):
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
        registers.PC = arg;
        break;
    case (0xD4):
        if ((registers.F & FLAG_CARY) == 0x0) {
            dec_16bit(registers.SP);
            gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
            dec_16bit(registers.SP);
            gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
            registers.PC = arg;
        }
        break;
    case (0xDC):
        if ((registers.F & FLAG_CARY) > 0x0) {
            dec_16bit(registers.SP);
            gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
            dec_16bit(registers.SP);
            gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
            registers.PC = arg;
        }
        break;
    default:
        break;
    }
}

void CPU::op_Restart(uint8_t opcode, uint16_t arg) {
    switch (opcode) {
    case (0xC7):
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
        registers.PC = 0x0000;
        break;
    case (0xCF):
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
        registers.PC = 0x0008;
        break;
    case (0xD7):
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
        registers.PC = 0x0010;
        break;
    case (0xDF):
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
        registers.PC = 0x0018;
        break;
    case (0xE7):
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
        registers.PC = 0x0020;
        break;
    case (0xEF):
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
        registers.PC = 0x0028;
        break;
    case (0xF7):
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
        registers.PC = 0x0030;
        break;
    case (0xFF):
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)(registers.PC >> 8));
        dec_16bit(registers.SP);
        gbmemory.set_memory(swap_endian(registers.SP), (uint8_t)registers.PC);
        registers.PC = 0x0038;
        break;
    default:
        break;
    }
}

void CPU::op_Return(uint8_t opcode, uint16_t arg) {
    uint8_t least, most;

    switch (opcode) {
    case (0xC0):
        if ((registers.F & FLAG_ZERO) == 0x0) {
            least = gbmemory.get_memory(registers.SP);
            inc_16bit(registers.SP);
            most = gbmemory.get_memory(registers.SP);
            dec_16bit(registers.SP);
            registers.PC = (most << 8) | least;
        }
        break;
    case (0xC8):
        if ((registers.F & FLAG_ZERO) > 0x0) {
            least = gbmemory.get_memory(registers.SP);
            inc_16bit(registers.SP);
            most = gbmemory.get_memory(registers.SP);
            dec_16bit(registers.SP);
            registers.PC = (most << 8) | least;
        }
        break;
    case (0xC9):
        least = gbmemory.get_memory(registers.SP);
        inc_16bit(registers.SP);
        most = gbmemory.get_memory(registers.SP);
        dec_16bit(registers.SP);
        registers.PC = (most << 8) | least;
        break;
    case (0xD0):
        if ((registers.F & FLAG_CARY) == 0x0) {
            least = gbmemory.get_memory(registers.SP);
            inc_16bit(registers.SP);
            most = gbmemory.get_memory(registers.SP);
            dec_16bit(registers.SP);
            registers.PC = (most << 8) | least;
        }
        break;
    case (0xD8):
        if ((registers.F & FLAG_CARY) > 0x0) {
            least = gbmemory.get_memory(registers.SP);
            inc_16bit(registers.SP);
            most = gbmemory.get_memory(registers.SP);
            dec_16bit(registers.SP);
            registers.PC = (most << 8) | least;
        }
        break;
    case (0xD9):
        least = gbmemory.get_memory(registers.SP);
        inc_16bit(registers.SP);
        most = gbmemory.get_memory(registers.SP);
        dec_16bit(registers.SP);
        registers.PC = (most << 8) | least;
        // TODO: enable interrupts
        break;
    }
}

void CPU::op_CB(uint8_t opcode, uint16_t arg) {
    (this->*CBops[opcode])(opcode, arg);
}

void CPU::op_Unknown(uint8_t opcode, uint16_t arg) {
    std::cout << "Unknown OPcode: " << opcode << "\n";
}