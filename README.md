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
---

## Lab 4 — The Shape of Control

### CPU control flow instructions

Implemented the following instructions:

| Instruction | Opcode | Description |
|---|---:|---|
| CMP | 0x1A | Compare registers A and B and update flags |
| JMP | 0x30 | Unconditional jump |
| JZ | 0x31 | Jump if zero flag is set |
| JNZ | 0x32 | Jump if zero flag is not set |

The CPU uses the `switch` statement in `step()` to select and execute instructions.

Unknown opcodes stop the CPU and display an error message.

### Conditional jumps

- `JMP` changes the program counter to the specified address.
- `JZ` jumps when `Z = 1`.
- `JNZ` jumps when `Z = 0`.
- A taken jump sets PC directly to the target address.
- A jump that is not taken advances PC by three bytes.

### Countdown program

The countdown program prints:

```text
3 2 1
```

Program listing:

| Address | Byte | Instruction |
|---:|---:|---|
| 0x000 | 0x20 | LOADI |
| 0x001 | 0x03 | Immediate value 3 |
| 0x002 | 0x03 | OUTN |
| 0x003 | 0x19 | DEC |
| 0x004 | 0x32 | JNZ |
| 0x005 | 0x02 | Target address low byte |
| 0x006 | 0x00 | Target address high byte |
| 0x007 | 0x00 | HALT |

### Countdown execution trace

| Instruction | A | Z | PC after execution |
|---|---:|---:|---:|
| LOADI 3 | 3 | 0 | 2 |
| OUTN | 3 | 0 | 3 |
| DEC | 2 | 0 | 4 |
| JNZ 2 | 2 | 0 | 2 |
| OUTN | 2 | 0 | 3 |
| DEC | 1 | 0 | 4 |
| JNZ 2 | 1 | 0 | 2 |
| OUTN | 1 | 0 | 3 |
| DEC | 0 | 1 | 4 |
| JNZ 2 | 0 | 1 | 7 |
| HALT | 0 | 1 | 8 |

### C++ control flow

The countdown demonstrates the same logic as a C++ `while` loop:

```cpp
int count = 3;

while (count > 0) {
    std::cout << count << ' ';
    count--;
}
```

The loop repeats while the condition is true and stops when the counter reaches zero.

### Memory search

Implemented the `find` command and the `find_byte()` helper function.

Example:

```text
ember> set 100 65
ember> find 100 110 65
found at address 100
```

The search checks memory addresses in the range from `lo` inclusive to `hi` exclusive.

The implementation uses a `for` loop and returns the address of the first matching byte. If the value is not found, it returns the end of the range.

### Execution limit

The `run` command has a 10,000-instruction execution limit to prevent an infinite program from running forever.

### Verification

The project builds successfully with:

```bash
cmake --build build
```

The countdown program produces the expected output:

```text
3 2 1
```
### Linear search inside Ember

The guest program searches for byte `0x41` in the eight-byte memory region starting at `0x800`.

The H register stores the current memory address. `INCH` moves to the next byte. `CMP` compares the loaded byte with the target value. `JZ` jumps to the result when the values match.

The program checks the end of the region at `0x808`. If the byte is not found, it prints `255`.

#### Program listing

| Address | Byte | Instruction |
|---:|---:|---|
| 0x000 | 0x28 | LOADH H |
| 0x001 | 0x00 | Address low byte |
| 0x002 | 0x08 | Address high byte |
| 0x003 | 0x21 | LOADI B |
| 0x004 | 0x41 | Search value |
| 0x005 | 0x29 | loop: LOAD A, [H] |
| 0x006 | 0x1A | CMP A, B |
| 0x007 | 0x31 | JZ found |
| 0x008 | 0x17 | Target low byte |
| 0x009 | 0x00 | Target high byte |
| 0x00A | 0x2B | INCH |
| 0x00B | 0x2D | HLOW |
| 0x00C | 0x21 | LOADI B |
| 0x00D | 0x08 | End address low byte |
| 0x00E | 0x1A | CMP A, B |
| 0x00F | 0x31 | JZ not_found |
| 0x010 | 0x1A | Target low byte |
| 0x011 | 0x00 | Target high byte |
| 0x012 | 0x21 | LOADI B |
| 0x013 | 0x41 | Search value |
| 0x014 | 0x30 | JMP loop |
| 0x015 | 0x05 | Loop address low byte |
| 0x016 | 0x00 | Loop address high byte |
| 0x017 | 0x2D | found: HLOW |
| 0x018 | 0x03 | OUTN |
| 0x019 | 0x00 | HALT |
| 0x01A | 0x20 | not_found: LOADI A |
| 0x01B | 0xFF | Not-found marker |
| 0x01C | 0x03 | OUTN |
| 0x01D | 0x00 | HALT |

#### Test data

The eight bytes are `10 20 30 41 50 60 70 80`.

Expected output: `3`, because `0x41` is at index 3 (address `0x803`).

If the value is absent, the program outputs `255`.
---

## Lab 5 — Arrays, Strings, Search and Screen

### Screen and video memory

Implemented a 64×32 monochrome screen.

The screen uses 256 bytes of the existing `Memory` array. Video memory (VRAM) occupies addresses `0xA00–0xAFF`.

Each pixel uses one bit:
- `#` — lit pixel.
- `.` — unlit pixel.

There are 8 bytes per row because each byte stores 8 pixels.

### Pixel address calculation

The byte address for pixel `(x, y)` is:

```text
address = 0xA00 + y * 8 + x / 8
```

The bit position inside the byte is:

```text
bit = 7 - (x % 8)
```

The most significant bit represents the leftmost pixel in each byte.

To turn a pixel on:

```cpp
mem.data[address] |= (1u << bit);
```

To turn a pixel off:

```cpp
mem.data[address] &= ~(1u << bit);
```

### Screen functions

Implemented in `src/screen.cpp`:

- `clear()` — clears all 256 bytes of video memory.
- `plot()` — turns on a pixel at the specified coordinates.
- `unplot()` — turns off a pixel.
- `show()` — prints the 64×32 screen using `#` and `.`.

Coordinates outside the screen boundaries are rejected.

### Screen commands

```text
ember> plot 0 0
ember> plot 10 5
ember> plot 63 31
ember> show
```

The screen displays three pixels at the specified coordinates.

The `clear` command clears the screen, and `show` displays the empty screen.

### CPU screen instructions

Added the following opcodes:

| Instruction | Opcode | Description |
|---|---:|---|
| OUTS | 0x04 | Print a null-terminated string, up to 256 bytes |
| CLS | 0x40 | Clear video memory |
| PLOT | 0x41 | Draw a pixel using A as x and B as y |
| UNPLOT | 0x42 | Clear a pixel using A as x and B as y |
| SHOW | 0x43 | Display the screen |

The CPU screen instructions use the same video memory as the host screen commands.

### OUTS string output

`OUTS` reads a string from guest memory until it encounters the null terminator (`0`) or reaches the 256-byte limit.

Test:

```text
ember> set 0 4
ember> set 1 0
ember> set 2 8
ember> set 3 0
ember> set 2048 72
ember> set 2049 69
ember> set 2050 76
ember> set 2051 76
ember> set 2052 79
ember> set 2053 0
ember> run
HELLO
```

### Verification

The project builds successfully:

```bash
cmake --build build
```

Verified:
- Drawing pixels at different screen coordinates.
- Clearing the screen.
- Rejecting invalid pixel coordinates.
- Executing `CLS`, `PLOT` and `SHOW` through the CPU.
- Printing `HELLO` using the `OUTS` instruction.

### Video memory size

A 64×32 screen contains 2048 pixels.

At one bit per pixel:

```text
64 * 32 = 2048 bits
2048 / 8 = 256 bytes
```

Video memory can also be inspected using the existing `dump` command.
## Lab 6 — Named Bundles: Structs, Enums and Heap

### 1. Enum class

У лабораторній роботі звичайні константи `OP_*` були замінені на перелічуваний тип `enum class Op : Byte`.

Кожна операція процесора має власне ім'я та числовий код. Значення кодів залишилися незмінними, тому сумісність із програмами попередніх лабораторних робіт збережена.

У функції `step()` отриманий із пам'яті байт перетворюється на тип `Op`:

```cpp
Byte raw_op = mem_get(*cpu.mem, cpu.pc);
Op op = static_cast<Op>(raw_op);
```

Після цього операція обробляється через `switch` із використанням назв `Op::HALT`, `Op::ADD`, `Op::SUB` та інших.

`enum class` забезпечує типобезпечність і не дозволяє випадково використовувати значення переліку як звичайні цілі числа без явного перетворення.

### 2. CPU structure size and memory alignment

Для перевірки розміру структур використано `sizeof`, `alignof` та `offsetof`.

Результати вимірювання на 64-бітній системі:

| Елемент | Розмір / зміщення |
|---|---:|
| `sizeof(Flags)` | 3 bytes |
| `sizeof(CPU)` | 24 bytes |
| `alignof(CPU)` | 8 bytes |
| `mem` offset | 0 |
| `pc` offset | 8 |
| `h` offset | 10 |
| `a` offset | 12 |
| `b` offset | 13 |
| `f` offset | 14 |
| `halted` offset | 17 |

Покажчик `mem` займає 8 байтів. Поля структури розташовуються з урахуванням вимог вирівнювання пам'яті (alignment).

Після останнього поля `halted` залишаються 6 байтів padding. Тому фактичний розмір структури `CPU` становить 24 байти, хоча самі поля займають 18 байтів.

### 3. Dynamic memory allocation

Динамічна пам'ять виділяється оператором `new`, а звільняється оператором `delete`.

Приклад:

```cpp
int* p = new int(42);
delete p;
```

Після `delete` пам'ять більше не належить програмі. Вказівник `p` стає висячим (dangling pointer), тому його не можна використовувати для доступу до звільненої пам'яті.

### 4. AddressSanitizer tests

Для демонстрації помилок роботи з динамічною пам'яттю було створено окремі тестові програми та перевірено їх за допомогою AddressSanitizer.

**Test 1 — Use-after-free**

```cpp
int main() {
    int* p = new int(42);
    delete p;
    return *p;
}
```

Після звільнення пам'яті програма намагається прочитати значення через вказівник `p`.

AddressSanitizer виявив помилку:

```text
ERROR: AddressSanitizer: heap-use-after-free
```

**Test 2 — Double-free**

```cpp
int main() {
    int* p = new int(42);
    delete p;
    delete p;
}
```

У цьому випадку одна й та сама ділянка пам'яті звільняється двічі.

AddressSanitizer виявив помилку:

```text
ERROR: AddressSanitizer: attempting double-free
SUMMARY: AddressSanitizer: double-free
```

Обидва тести підтверджують, що AddressSanitizer допомагає виявляти помилки керування динамічною пам'яттю.

### 5. Conclusion

Під час лабораторної роботи було замінено константи операцій на `enum class Op`, перевірено розмір і вирівнювання структур `Flags` та `CPU`, а також досліджено виділення і звільнення динамічної пам'яті.

За допомогою AddressSanitizer було продемонстровано помилки `heap-use-after-free` та `double-free`.