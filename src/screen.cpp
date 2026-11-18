#include "screen.hpp"

#include <iostream>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;
const int VRAM_LO = 0xA00;

void clear(Memory& mem) {
    for (int i = 0; i < 256; ++i) {
        mem.data[VRAM_LO + i] = 0;
    }
}

bool plot(Memory& mem, int x, int y) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return false;
    }

    int address = VRAM_LO + y * 8 + x / 8;
    int bit = 7 - (x % 8);
    mem.data[address] |= (1u << bit);
    return true;
}

bool unplot(Memory& mem, int x, int y) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return false;
    }

    int address = VRAM_LO + y * 8 + x / 8;
    int bit = 7 - (x % 8);
    mem.data[address] &= ~(1u << bit);
    return true;
}

void show(const Memory& mem) {
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            Byte cell = mem.data[VRAM_LO + y * 8 + x / 8];
            bool lit = (cell >> (7 - (x % 8))) & 1;
            std::cout << (lit ? '#' : '.');
        }
        std::cout << '\n';
    }
}
