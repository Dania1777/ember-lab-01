// main.cpp — the prompt.
//
// GIVEN, in full. This file is scaffolding: it reads a line, splits it into
// words, and calls one of your functions. It uses `while`, `if` and functions,
// which the course only explains properly in Labs 4 and 7. That is on purpose —
// in Lab 1 you read this file, you do not write it.
//
// What you add in later labs is one more `else if` branch per command.
#include <iostream>
#include <sstream>
#include <string>

#include "dump.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include "alu.hpp"
#include "screen.hpp"

// Turn a word into a number. Accepts decimal (65) and hex (0x41).
// Returns false if the word is not a number at all.
static bool parse_number(const std::string& word, long& out) {
    try {
        std::size_t used = 0;
        // base 0 means: look at the prefix. "0x41" is hex, "65" is decimal.
        out = std::stol(word, &used, 0);
        return used == word.size();  // reject things like "12abc"
    } catch (...) {
        return false;
    }
}
static std::uint16_t find_byte(
    const Memory& mem,
    std::uint16_t lo,
    std::uint16_t hi,
    Byte needle
) {
    for (std::uint16_t i = lo; i < hi; ++i) {
        if (mem_get(mem, i) == needle) {
            return i;
        }
    }

    return hi;
}

static void print_help() {
    std::cout << "commands:\n"
              << "  dump              print all " << MEM_SIZE << " bytes\n"
              << "  find <lo> <hi> <val> search for a byte\n"
              << "  clear             clear the screen\n"
              << "  plot <x> <y>       light one pixel\n"
              << "  show              print the screen\n"
              << "  get <addr>        show one byte four ways\n"
              << "  set <addr> <val>  write one byte (dec or 0x hex)\n"
              << "  inc <addr>        increment byte at address\n"
              << "  reg <a|b> <val>   set CPU register A or B\n"
              << "  regs              show CPU registers and flags\n"
              << "  alu <op> <a> [b]  test an ALU operation\n"
              << "  step              execute one CPU instruction\n"
              << "  run               execute until HALT\n"
              << "  help              this list\n"
              << "  quit              leave\n";
}


int main() {
    Memory mem;  // 4096 bytes, on the stack, zeroed by the {} in memory.hpp
    CPU cpu;
cpu.mem = &mem;

    std::cout << "ember 0.1 - 4096 bytes of memory you can see. Type `help`.\n";

    std::string line;
    while (true) {
        std::cout << "ember> ";

        // getline reads one whole line. It returns false at end of input
        // (Ctrl-D), which is how the loop ends if you never type `quit`.
        if (!std::getline(std::cin, line)) {
            std::cout << '\n';
            break;
        }

        // Split the line into words: "set 0 65" -> cmd="set", args "0" and "65".
        std::istringstream words(line);
        std::string cmd;
        words >> cmd;

        if (cmd.empty()) {
            continue;  // the user just pressed Enter
        } else if (cmd == "quit" || cmd == "exit") {
            break;
        } else if (cmd == "help") {
            print_help();
        } else if (cmd == "dump") {
            dump(mem);
            } else if (cmd == "clear") {
    clear(mem);
} else if (cmd == "plot") {
    std::string sx, sy;
    long x = 0, y = 0;

    if (!(words >> sx >> sy) ||
        !parse_number(sx, x) || !parse_number(sy, y)) {
        std::cout << "usage: plot <x> <y>\n";
    } else if (!plot(mem, static_cast<int>(x), static_cast<int>(y))) {
        std::cout << "coordinates must be x=0..63, y=0..31\n";
    }
} else if (cmd == "show") {
    show(mem);
        } else if (cmd == "get") {
            std::string a;
            long addr = 0;
            if (!(words >> a) || !parse_number(a, addr)) {
                std::cout << "usage: get <addr>\n";
            } else if (addr < 0) {
                std::cout << "address must not be negative\n";
            } else {
                show_byte(mem_get(mem, (std::size_t)addr));
            }
        } else if (cmd == "set") {
            std::string a, v;
            long addr = 0, value = 0;
            if (!(words >> a) || !(words >> v) || !parse_number(a, addr) ||
                !parse_number(v, value)) {
                std::cout << "usage: set <addr> <value>\n";
            } else if (addr < 0) {
                std::cout << "address must not be negative\n";
            } else if (value < 0 || value > 255) {
                // A cell holds ONE byte. 256 does not fit. Lab 1, theory 3.
                std::cout << "a byte is 0..255, got " << value << '\n';

            } else if (!mem_set(mem, (std::size_t)addr, (Byte)value)) {
                std::cout << "address " << addr << " is outside 0.." << MEM_SIZE - 1
                          << '\n';
            }
        } else if (cmd == "reg") {
            std::string r, v;
            long value = 0;

            if (!(words >> r) || !(words >> v) ||
                !parse_number(v, value) || value < 0 || value > 255) {
                std::cout << "usage: reg <a|b> <0..255>\n";
            } else if (r == "a") {
                cpu.a = (Byte)value;
            } else if (r == "b") {
                cpu.b = (Byte)value;
                        } else {
                std::cout << "unknown register: " << r << '\n';
            }

        } else if (cmd == "alu") {
            std::string operation, sa, sb;
            long a = 0, b = 0;
            if (!(words >> operation) || !(words >> sa) ||
                !parse_number(sa, a) || a < 0 || a > 255) {
                std::cout << "usage: alu <add|sub|and|or|xor|not|shl|shr> <a> [b]\n";
            } else {
                Flags flags;
                Byte result = 0;
                bool valid = true;

                if (operation == "not" || operation == "shl" ||
                    operation == "shr") {
                    Byte value = (Byte)a;
                    if (operation == "not") result = alu_not(value, flags);
                    else if (operation == "shl") result = alu_shl(value, flags);
                    else result = alu_shr(value, flags);
                } else {
                    if (!(words >> sb) || !parse_number(sb, b) ||
                        b < 0 || b > 255) {
                        std::cout << "usage: alu <add|sub|and|or|xor> <a> <b>\n";
                        valid = false;
                    } else if (operation == "add") result = alu_add((Byte)a, (Byte)b, flags);
                    else if (operation == "sub") result = alu_sub((Byte)a, (Byte)b, flags);
                    else if (operation == "and") result = alu_and((Byte)a, (Byte)b, flags);
                    else if (operation == "or") result = alu_or((Byte)a, (Byte)b, flags);
                    else if (operation == "xor") result = alu_xor((Byte)a, (Byte)b, flags);
                    else {
                        std::cout << "unknown ALU operation\n";
                        valid = false;
                    }
                }

                if (valid) {
                    std::cout << "result=" << (int)result
                              << " Z=" << flags.z
                              << " N=" << flags.n
                              << " C=" << flags.c << '\n';
                }
            }

                } else if (cmd == "regs") {
            dump_regs(cpu);
        } else if (cmd == "step") {
    step(cpu);
} else if (cmd == "run") {
    int count = 0;

    while (!cpu.halted && count < 10000) {
        step(cpu);
        count++;
    }

    if (!cpu.halted) {
        std::cout << "\nexecution limit reached\n";
    } else {
        std::cout << '\n';
    }
} else if (cmd == "inc") {

        std::string a;
        long addr = 0;
        if (!(words >> a) || !parse_number(a, addr)) {
            std::cout << "usage: inc <addr>\n";
        } else if (addr < 0) {
            std::cout << "address must not be negative\n";
        } else {
            Byte value = mem_get(mem, (std::size_t)addr);
            value += 1;
            mem_set(mem, (std::size_t)addr, value);
                }
        } else if (cmd == "find") {
            std::string slo, shi, sbyte;
            long lo = 0, hi = 0, value = 0;

            if (!(words >> slo >> shi >> sbyte) ||
                !parse_number(slo, lo) ||
                !parse_number(shi, hi) ||
                !parse_number(sbyte, value)) {
                std::cout << "usage: find <lo> <hi> <byte>\n";
            } else if (lo < 0 || hi < lo || hi > static_cast<long>(MEM_SIZE) ||
                       value < 0 || value > 255) {
                std::cout << "invalid range or byte value\n";
            } else {
                std::uint16_t result = find_byte(
                    mem,
                    static_cast<std::uint16_t>(lo),
                    static_cast<std::uint16_t>(hi),
                    static_cast<Byte>(value)
                );

                if (result == hi) {
                    std::cout << "not found\n";
                } else {
                    std::cout << "found at address " << result << '\n';
                }
            }
        } else {
            std::cout << "unknown command: " << cmd << " (try `help`)\n";
        }
    }

    return 0;  // 0 means "success" to the operating system
}
