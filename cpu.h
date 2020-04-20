#ifndef CPU_H
#define CPU_H

// C++ libraries
#include <cstdint>
#include <cstddef>
#include <functional>

// GBemu sources
#include "memory.h"

class CPU {
    public:
    const uint8_t FLAG_ZERO = 0b10000000;
    const uint8_t FLAG_ADSB = 0b01000000;
    const uint8_t FLAG_HALF = 0b00100000;
    const uint8_t FLAG_CARY = 0b00010000;

    struct registerMap {
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

    void (CPU::*opcodes[0x100])(uint8_t, uint16_t);
    void (CPU::*CBops[0x100])(uint8_t, uint16_t);
    Memory gbmemory;

    CPU(Memory &mem);
    uint16_t swap_endian(uint16_t bytes);
    uint16_t concat_regist(uint8_t most, uint8_t least);
    void dec_16bit(uint8_t &most, uint8_t &least);
    void inc_16bit(uint8_t &most, uint8_t &least);
    void op_Load(uint8_t opcode, uint16_t arg);
    void op_Push(uint8_t opcode, uint16_t arg);
    void op_Pop(uint8_t opcode, uint16_t arg);
    void op_Add(uint8_t opcode, uint16_t arg);
    void op_Subtract(uint8_t opcode, uint16_t arg);
    void op_And(uint8_t opcode, uint16_t arg);
    void op_Or(uint8_t opcode, uint16_t arg);
    void op_Xor(uint8_t opcode, uint16_t arg);
    void op_Compare(uint8_t opcode, uint16_t arg);
    void op_Increment(uint8_t opcode, uint16_t arg);
    void op_Decrement(uint8_t opcode, uint16_t arg);
    void op_Swap(uint8_t opcode, uint16_t arg);
    void op_Decimal(uint8_t opcode, uint16_t arg);
    void op_Complement(uint8_t opcode, uint16_t arg);
    void op_CompCarry(uint8_t opcode, uint16_t arg);
    void op_Carry(uint8_t opcode, uint16_t arg);
    void op_Nop(uint8_t opcode, uint16_t arg);
    void op_Halt(uint8_t opcode, uint16_t arg);
    void op_Stop(uint8_t opcode, uint16_t arg);
    void op_DInterrupt(uint8_t opcode, uint16_t arg);
    void op_EInterrupt(uint8_t opcode, uint16_t arg);
    void op_Rotate(uint8_t opcode, uint16_t arg);
    void op_Shift(uint8_t opcode, uint16_t arg);
    void op_Bit(uint8_t opcode, uint16_t arg);
    void op_Jump(uint8_t opcode, uint16_t arg);
    void op_Call(uint8_t opcode, uint16_t arg);
    void op_Restart(uint8_t opcode, uint16_t arg);
    void op_Return(uint8_t opcode, uint16_t arg);
    void op_CB(uint8_t opcode, uint16_t arg);
    void op_Unknown(uint8_t opcode, uint16_t arg);
};

#endif