
#pragma once

#include <cstdint>
#include "memory.hpp"

enum class Op : Byte {
    HALT = 0x00, NOP = 0x01,
    OUT = 0x02, OUTN = 0x03, OUTS = 0x04,
    ADD = 0x10, SUB = 0x11, AND = 0x12, OR = 0x13,
    XOR = 0x14, NOT = 0x15, SHL = 0x16, SHR = 0x17,
    INC = 0x18, DEC = 0x19, CMP = 0x1A,
    LOADI_A = 0x20, LOADI_B = 0x21,
    LOAD_A = 0x22, LOAD_B = 0x23,
    STORE_A = 0x24, STORE_B = 0x25,
    MOV_A_B = 0x26, MOV_B_A = 0x27,
    LOADH = 0x28, LOAD_A_H = 0x29, STORE_A_H = 0x2A,
    INCH = 0x2B, DECH = 0x2C, HLOW = 0x2D,
    JMP = 0x30, JZ = 0x31, JNZ = 0x32,
    CLS = 0x40, PLOT = 0x41, UNPLOT = 0x42, SHOW = 0x43
};
struct Flags {
    bool z = false;
    bool n = false;
    bool c = false;
};

struct CPU {
    Memory* mem = nullptr;
    std::uint16_t pc = 0;
    std::uint16_t h = 0;
    Byte a = 0;
    Byte b = 0;
    Flags f;
    bool halted = false;
};

void step(CPU& cpu);
void dump_regs(const CPU& cpu);
