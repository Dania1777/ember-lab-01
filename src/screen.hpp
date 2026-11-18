#pragma once

#include "memory.hpp"

void clear(Memory& mem);
bool plot(Memory& mem, int x, int y);
bool unplot(Memory& mem, int x, int y);
void show(const Memory& mem);
