/*
 * Sharp LR35902
 * 8-bit @ 4.19 Mhz
 */
#include <cstdint>
#include <cstddef>
#include <functional>

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

        // Array mapping OP code (as index) to function for handling the OP code
        void (emu::CPU::*opcodes[0x100])(uint8_t opcode) = {
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
        void (emu::CPU::*CBops[0x100])(uint8_t opcode) = {
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

        // Initialize registers to boot-up state
        CPU() {
            registers.AF = 0x01B0;
            registers.BC = 0x0013;
            registers.DE = 0x00D8;
            registers.HL = 0x014D;
            registers.SP = 0xFFFE;
            registers.PC = 0x100;
        }

        // Load OP codes
        void op_Load(uint8_t opcode) {

        }

        void op_Push(uint8_t opcode) {

        }

        void op_Pop(uint8_t opcode) {

        }

        void op_Add(uint8_t opcode) {

        }

        void op_Subtract(uint8_t opcode) {

        }

        void op_And(uint8_t opcode) {

        }

        void op_Or(uint8_t opcode) {

        }

        void op_Xor(uint8_t opcode) {

        }

        void op_Compare(uint8_t opcode) {

        }

        void op_Increment(uint8_t opcode) {

        }

        void op_Decrement(uint8_t opcode) {

        }

        void op_Swap(uint8_t opcode) {

        }

        void op_Decimal(uint8_t opcode) {

        }

        void op_Compliment(uint8_t opcode) {

        }

        void op_CompCarry(uint8_t opcode) {

        }

        void op_Carry(uint8_t opcode) {

        }

        void op_Nop(uint8_t opcode) {

        }

        void op_Halt(uint8_t opcode) {

        }

        void op_Stop(uint8_t opcode) {

        }

        void op_DInterrupt(uint8_t opcode) {

        }

        void op_EInterrupt(uint8_t opcode) {

        }

        void op_Rotate(uint8_t opcode) {

        }

        void op_Shift(uint8_t opcode) {

        }

        void op_Bit(uint8_t opcode) {

        }

        void op_Jump(uint8_t opcode) {

        }

        void op_Call(uint8_t opcode) {

        }

        void op_Restart(uint8_t opcode) {

        }

        void op_Return(uint8_t opcode) {

        }

        void op_CB(uint8_t opcode) {

        }

        void op_Unknown(uint8_t opcode) {

        }
    };
}