
#include "cpu.hpp"
#include "alu.hpp"
#include "screen.hpp"
#include <iostream>


void step(CPU& cpu) {
    if (cpu.halted) {
        std::cout << "halted\n";
        return;
    }

    Byte raw_op = mem_get(*cpu.mem, cpu.pc);
    Op op = static_cast<Op>(raw_op);

    switch (op) {
    case Op::HALT:
        cpu.halted = true;
        cpu.pc += 1;
        break;

    case Op::NOP:
        cpu.pc += 1;
        break;

    case Op::OUT:
        std::cout << static_cast<char>(cpu.a);
        cpu.pc += 1;
        break;

    case Op::OUTN:
        std::cout << static_cast<int>(cpu.a) << ' ';
        cpu.pc += 1;
        break;

    case Op::OUTS: {
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

    case Op::LOADI_A:
        cpu.a = mem_get(*cpu.mem, cpu.pc + 1);
        cpu.pc += 2;
        break;

    case Op::LOADI_B:
        cpu.b = mem_get(*cpu.mem, cpu.pc + 1);
        cpu.pc += 2;
        break;

    case Op::LOAD_A:
        cpu.a = mem_get(*cpu.mem, get16(*cpu.mem, cpu.pc + 1));
        cpu.pc += 3;
        break;

    case Op::LOAD_B:
        cpu.b = mem_get(*cpu.mem, get16(*cpu.mem, cpu.pc + 1));
        cpu.pc += 3;
        break;

    case Op::STORE_A:
        mem_set(*cpu.mem, get16(*cpu.mem, cpu.pc + 1), cpu.a);
        cpu.pc += 3;
        break;

    case Op::STORE_B:
        mem_set(*cpu.mem, get16(*cpu.mem, cpu.pc + 1), cpu.b);
        cpu.pc += 3;
        break;

    case Op::MOV_A_B:
        cpu.a = cpu.b;
        cpu.pc += 1;
        break;

    case Op::MOV_B_A:
        cpu.b = cpu.a;
        cpu.pc += 1;
        break;

    case Op::LOADH:
        cpu.h = get16(*cpu.mem, cpu.pc + 1);
        cpu.pc += 3;
        break;

    case Op::LOAD_A_H:
        cpu.a = mem_get(*cpu.mem, cpu.h);
        cpu.pc += 1;
        break;

    case Op::STORE_A_H:
        mem_set(*cpu.mem, cpu.h, cpu.a);
        cpu.pc += 1;
        break;

    case Op::INCH:
        cpu.h += 1;
        cpu.pc += 1;
        break;

    case Op::DECH:
        cpu.h -= 1;
        cpu.pc += 1;
        break;

    case Op::ADD:
        cpu.a = alu_add(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case Op::SUB:
        cpu.a = alu_sub(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case Op::AND:
        cpu.a = alu_and(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case Op::OR:
        cpu.a = alu_or(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case Op::XOR:
        cpu.a = alu_xor(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case Op::NOT:
        cpu.a = alu_not(cpu.a, cpu.f);
        cpu.pc += 1;
        break;

    case Op::SHL:
        cpu.a = alu_shl(cpu.a, cpu.f);
        cpu.pc += 1;
        break;

    case Op::SHR:
        cpu.a = alu_shr(cpu.a, cpu.f);
        cpu.pc += 1;
        break;

    case Op::INC:
        cpu.a = alu_add(cpu.a, 1, cpu.f);
        cpu.pc += 1;
        break;

    case Op::DEC:
        cpu.a = alu_sub(cpu.a, 1, cpu.f);
        cpu.pc += 1;
        break;

    case Op::CMP:
        alu_sub(cpu.a, cpu.b, cpu.f);
        cpu.pc += 1;
        break;

    case Op::HLOW:
        cpu.a = static_cast<Byte>(cpu.h & 0xFF);
        cpu.pc += 1;
        break;

    case Op::JMP:
        cpu.pc = get16(*cpu.mem, cpu.pc + 1);
        break;

    case Op::JZ:
        if (cpu.f.z) {
            cpu.pc = get16(*cpu.mem, cpu.pc + 1);
        } else {
            cpu.pc += 3;
        }
        break;

    case Op::JNZ:
        if (!cpu.f.z) {
            cpu.pc = get16(*cpu.mem, cpu.pc + 1);
        } else {
            cpu.pc += 3;
        }
        break;

    case Op::CLS:
        clear(*cpu.mem);
        cpu.pc += 1;
        break;

    case Op::PLOT:
        cpu.f.c = !plot(*cpu.mem, cpu.a, cpu.b);
        cpu.pc += 1;
        break;

    case Op::UNPLOT:
        cpu.f.c = !unplot(*cpu.mem, cpu.a, cpu.b);
        cpu.pc += 1;
        break;

    case Op::SHOW:
        show(*cpu.mem);
        cpu.pc += 1;
        break;

    default:
        std::cout << "unknown opcode 0x"
                  << std::hex << static_cast<int>(raw_op)
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
