
#include "cpu.hpp"
#include "alu.hpp"
#include <iostream>

void step(CPU& cpu) {
    if (cpu.halted) {
        std::cout << "halted\n";
        return;
    }

    Byte op = mem_get(*cpu.mem, cpu.pc);

    switch (op) {
    case OP_HALT:
        cpu.halted = true;
        cpu.pc += 1;
        break;

    case OP_NOP:
        cpu.pc += 1;
        break;

    case OP_ADD:
        cpu.a = alu_add(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case OP_SUB:
        cpu.a = alu_sub(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case OP_AND:
        cpu.a = alu_and(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case OP_OR:
        cpu.a = alu_or(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case OP_XOR:
        cpu.a = alu_xor(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case OP_NOT:
        cpu.a = alu_not(cpu.a, cpu.f);
        cpu.pc += 1;
        break;

    case OP_SHL:
        cpu.a = alu_shl(cpu.a, cpu.f);
        cpu.pc += 1;
        break;

    case OP_SHR:
        cpu.a = alu_shr(cpu.a, cpu.f);
        cpu.pc += 1;
        break;

    case OP_INC:
        cpu.a = alu_add(cpu.a, 1, cpu.f);
        cpu.pc += 1;
        break;

    case OP_DEC:
        cpu.a = alu_sub(cpu.a, 1, cpu.f);
        cpu.pc += 1;
        break;

    default:
        cpu.pc += 1;
        break;
    }
}

void dump_regs(const CPU& cpu) {
    std::cout << "PC=" << cpu.pc
              << " A=" << static_cast<int>(cpu.a)
              << " B=" << static_cast<int>(cpu.b)
              << " Z=" << cpu.f.z
              << " N=" << cpu.f.n
              << " C=" << cpu.f.c
              << '\n';
}
