
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
    
        case OP_OUT:
        std::cout << static_cast<char>(cpu.a);
        cpu.pc += 1;
        break;

    case OP_OUTN:
        std::cout << static_cast<int>(cpu.a);
        cpu.pc += 1;
        break;
        case OP_LOADI_A:
        cpu.a = mem_get(*cpu.mem, cpu.pc + 1);
        cpu.pc += 2;
        break;

    case OP_LOADI_B:
        cpu.b = mem_get(*cpu.mem, cpu.pc + 1);
        cpu.pc += 2;
        break;
        
        case OP_LOAD_A:
        cpu.a = mem_get(*cpu.mem, get16(*cpu.mem, cpu.pc + 1));
        cpu.pc += 3;
        break;

    case OP_LOAD_B:
        cpu.b = mem_get(*cpu.mem, get16(*cpu.mem, cpu.pc + 1));
        cpu.pc += 3;
        break;
        case OP_STORE_A:
        mem_set(*cpu.mem, get16(*cpu.mem, cpu.pc + 1), cpu.a);
        cpu.pc += 3;
        break;

    case OP_STORE_B:
        mem_set(*cpu.mem, get16(*cpu.mem, cpu.pc + 1), cpu.b);
        cpu.pc += 3;
        break;
        case OP_MOV_A_B:
        cpu.a = cpu.b;
        cpu.pc += 1;
        break;

    case OP_MOV_B_A:
        cpu.b = cpu.a;
        cpu.pc += 1;
        break;

        case OP_LOADH:
        cpu.h = get16(*cpu.mem, cpu.pc + 1);
        cpu.pc += 3;
        break;
        case OP_LOAD_A_H:
        cpu.a = mem_get(*cpu.mem, cpu.h);
        cpu.pc += 1;
        break;

    case OP_STORE_A_H:
        mem_set(*cpu.mem, cpu.h, cpu.a);
        cpu.pc += 1;
        break;
        case OP_INCH:
        cpu.h += 1;
        cpu.pc += 1;
        break;

    case OP_DECH:
        cpu.h -= 1;
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
              << " H=" << cpu.h
              << " A=" << static_cast<int>(cpu.a)
              << " B=" << static_cast<int>(cpu.b)
              << " Z=" << cpu.f.z
              << " N=" << cpu.f.n
              << " C=" << cpu.f.c
              << '\n';
}
