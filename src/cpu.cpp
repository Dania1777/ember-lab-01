
#include "cpu.hpp"
#include "alu.hpp"
#include "screen.hpp"
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
    std::cout << static_cast<int>(cpu.a) << ' ';
    cpu.pc += 1;
    break;
    case OP_OUTS: {
    std::uint16_t addr = get16(*cpu.mem, cpu.pc + 1);

    for (int i = 0; i < 256; ++i) {
        Byte ch = mem_get(*cpu.mem, static_cast<std::size_t>(addr) + i);
        if (ch == 0) {
            break;
        }
        std::cout << static_cast<char>(ch);
    }

    cpu.pc += 3;
    break;
}
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

        case OP_CMP:
        alu_sub(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case OP_HLOW:
        cpu.a = static_cast<Byte>(cpu.h & 0xFF);
        cpu.pc += 1;
        break;

    case OP_JMP:
        cpu.pc = get16(*cpu.mem, cpu.pc + 1);
        break;

    case OP_JZ:
        if (cpu.f.z) {
            cpu.pc = get16(*cpu.mem, cpu.pc + 1);
        } else {
            cpu.pc += 3;
        }
        break;

    case OP_JNZ:
        if (!cpu.f.z) {
            cpu.pc = get16(*cpu.mem, cpu.pc + 1);
        } else {
            cpu.pc += 3;
        }
        break;
        case OP_CLS:
    clear(*cpu.mem);
    cpu.pc += 1;
    break;

case OP_PLOT:
    cpu.f.c = !plot(*cpu.mem, cpu.a, cpu.b);
    cpu.pc += 1;
    break;

case OP_UNPLOT:
    cpu.f.c = !unplot(*cpu.mem, cpu.a, cpu.b);
    cpu.pc += 1;
    break;

case OP_SHOW:
    show(*cpu.mem);
    cpu.pc += 1;
    break;

    default:
        std::cout << "unknown opcode 0x"
                  << std::hex << static_cast<int>(op)
                  << std::dec << '\n';
        cpu.halted = true;
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
