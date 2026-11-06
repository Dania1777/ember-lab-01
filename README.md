# `ember` — starter skeleton (Lab 1)

Copy this folder to a repository of your own, `git init`, and start from **M1** of
[Lab 01](../lab-01-a-box-of-bytes.md). It builds and runs as-is.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/ember
```

## Why a skeleton exists

Lab 1 is about **bytes and types**. It is not about `while` loops, splitting a
string into words, or `std::setw` — you meet those properly in Labs 4, 5 and 7.
So the parts that need them are given to you, fully written and commented. You
read those. You write the four small things that *are* Lab 1.

## Given — read it, don't rewrite it

| File | What it does |
|---|---|
| `CMakeLists.txt` | C++17, `-Wall -Wextra -Werror`, ASan + UBSan on Debug |
| `src/main.cpp` | the prompt: read a line, split it into words, call your functions |
| `src/memory.hpp` | `Byte`, `MEM_SIZE`, `struct Memory` — the box |
| `src/dump.hpp` | the two declarations |
| `src/dump.cpp` → `dump()` | the hex dump loop |

## Yours — four `TODO(lab-01)` markers

```bash
grep -rn "TODO(lab-01)" src/
```

| # | Where | The job |
|---|---|---|
| 1 | `memory.cpp` → `mem_get` | return the byte, or 0 if the address is outside the box |
| 2 | `memory.cpp` → `mem_set` | write the byte, or return `false` if the address is outside |
| 3 | `dump.cpp` → the ASCII gutter | print the character when the byte is printable |
| 4 | `dump.cpp` → `show_byte` | one byte, four views |

When all four are done:

```txt
ember> set 0 65
ember> set 1 66
ember> get 0
65  0x41  0b01000001  'A'
ember> dump
0000  41 42 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |AB..............|
```

That is M2 and M3 of Lab 1. M4 (the three deliberate breakages) is in the lab.

Until you implement `mem_set`, `set` accepts everything and stores nothing, and
`get` prints `show_byte: not implemented yet`. That is the starting state, not a
bug.

## Later labs

You keep this repository for all eight labs. Every lab adds one `else if` branch
to the dispatcher in `main.cpp` and one or two new files next to these.

---

## Українською

Скопіюйте цю теку у свій репозиторій — вона вже збирається й запускається.

Lab 1 — про **байти й типи**, а не про цикли, розбір рядка на слова чи
форматування виводу (це Labs 4, 5, 7). Тому все, що потребує ще не пройденого,
вам **дано** — з коментарями, щоб читати. Ви пишете чотири маленькі речі, які й
є Lab 1: `mem_get`, `mem_set`, ASCII-колонку в дампі та `show_byte`.

Знайти свою роботу: `grep -rn "TODO(lab-01)" src/`.

Якщо C++ бачите вперше — спочатку
[C++ за годину](../cpp-survival-kit.notes.md), потім
[інструменти й git](../setup.notes.md).
## Lab 1 results

### Memory
4096 bytes of memory. Each cell stores one byte (0..255).

### get
set 0 65
get 0
65  0x41  0b01000001  'A'

### Bounds check
set 5000 1
address 5000 is outside 0..4095

### inc
set 0 255
inc 0
get 0
0  0x0  0b00000000  '.'

### Floating point
0.1 + 0.2 = 0.3
0.1 + 0.2 == 0.3 → false

### C-string
65, 66, 0 produced:
|AB..............|

### Uninitialized memory
Removing {} from Byte data[MEM_SIZE] produced garbage bytes.
Restoring {} zero-initialized the memory again.
### sizeof

| Type | Size |
|---|---:|
| char | 1 |
| int | 4 |
| float | 4 |
| double | 8 |
| void* | 8 |

### Fresh dump

0000  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |................|
0010  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |................|
0020  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |................|

---

## Lab 2 — Bits Don't Lie: ALU and CPU

### ALU operations

The ALU works with 8-bit values (0..255).

| Operation | Opcode | Description |
|---|---:|---|
| HALT | 0x00 | Stop the CPU |
| NOP | 0x01 | Do nothing |
| ADD | 0x10 | Add A and B |
| SUB | 0x11 | Subtract B from A |
| AND | 0x12 | Bitwise AND |
| OR | 0x13 | Bitwise OR |
| XOR | 0x14 | Bitwise XOR |
| NOT | 0x15 | Invert all bits of A |
| SHL | 0x16 | Shift A left by one bit |
| SHR | 0x17 | Shift A right by one bit |
| INC | 0x18 | Increment A |
| DEC | 0x19 | Decrement A |

### Carry examples

```text
ember> alu add 200 100
result=44 Z=0 N=0 C=1

ember> alu add 255 1
result=0 Z=1 N=0 C=1
```

The result is stored in one byte. Overflow beyond 255 sets C=1.

### Logical operation examples

```text
ember> alu and 12 10
result=8 Z=0 N=0 C=0

ember> alu shl 5
result=10 Z=0 N=0 C=0
```

### CPU instruction trace

### CPU instruction trace

```text
ember> reg a 7
ember> reg b 1
ember> set 0 18
ember> set 1 22
ember> set 2 0
ember> step
ember> regs
PC=1 A=1 B=1 Z=0 N=0 C=0
ember> step
ember> regs
PC=2 A=2 B=1 Z=0 N=0 C=0
ember> step
ember> regs
PC=3 A=2 B=1 Z=0 N=0 C=0
```

The first instruction performs AND (opcode 18 decimal). The second instruction performs SHL (opcode 22 decimal). The third instruction is HALT (opcode 0). PC advances by one byte for each instruction.
---

## Lab 3 — Addresses, not names

### 16-bit memory access

Implemented `get16` and `set16` using little-endian byte order:
the lower byte is stored first, followed by the higher byte.

For example, `0x1234` is stored as `34 12`.

### CPU instructions

Implemented the instructions from the `0x2_` group:
`LOADI`, `LOAD`, `STORE`, `MOV`, `LOADH`, `INCH` and `DECH`.

Also implemented `OUT`, `OUTN` and the `run` command.
The `run` command executes instructions until `HALT` or the execution limit.

### Memory regions and H register

The program is stored starting at address `0x000`.
Data is stored starting at address `0x800`.

The program was tested using both absolute memory addresses and the H address register.
The H-register test produced the output:

```text
AB
```
### Register trace

After executing the program, the registers showed:

PC=9 H=2049 A=66 B=0 Z=0 N=0 C=0

### Host pointers and guest addresses

`Memory* mem` is a host pointer to the actual C++ memory object.

`pc` and `h` are guest addresses. They are 16-bit numbers used by the emulated computer to access its own memory.

A host pointer and a guest address are different things and must not be confused.

### AddressSanitizer experiment

A temporary program attempted to write beyond the memory array:

`mem.data[MEM_SIZE] = 1;`

AddressSanitizer detected:

`ERROR: AddressSanitizer: stack-buffer-overflow`

This means the program attempted to write outside the memory array. The experiment was performed in a separate temporary file, and the bounds checks in Ember remained in place.

### Verification

The official Lab 3 checks passed. The program produced:

`AB`

The final register state was:

`PC=9 H=2049 A=66 B=0 Z=0 N=0 C=0`