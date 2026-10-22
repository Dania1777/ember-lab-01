#include "alu.hpp"

static void update_zn(Byte result, Flags& f) {
    f.z = (result == 0);
    f.n = (result & 0x80) != 0;
}

Byte alu_add(Byte a, Byte b, Flags& f) {
    unsigned int sum = a + b;
    Byte result = static_cast<Byte>(sum);

    update_zn(result, f);
    f.c = sum > 255;

    return result;
}

Byte alu_sub(Byte a, Byte b, Flags& f) {
    Byte result = static_cast<Byte>(a - b);

    update_zn(result, f);
    f.c = a < b;

    return result;
}

Byte alu_and(Byte a, Byte b, Flags& f) {
    Byte result = a & b;

    update_zn(result, f);
    f.c = false;

    return result;
}

Byte alu_or(Byte a, Byte b, Flags& f) {
    Byte result = a | b;

    update_zn(result, f);
    f.c = false;

    return result;
}

Byte alu_xor(Byte a, Byte b, Flags& f) {
    Byte result = a ^ b;

    update_zn(result, f);
    f.c = false;

    return result;
}

Byte alu_not(Byte a, Flags& f) {
    Byte result = static_cast<Byte>(~a);

    update_zn(result, f);
    f.c = false;

    return result;
}

Byte alu_shl(Byte a, Flags& f) {
    f.c = (a & 0x80) != 0;
    Byte result = static_cast<Byte>(a << 1);

    update_zn(result, f);

    return result;
}

Byte alu_shr(Byte a, Flags& f) {
    f.c = (a & 0x01) != 0;
    Byte result = static_cast<Byte>(a >> 1);

    update_zn(result, f);

    return result;
}