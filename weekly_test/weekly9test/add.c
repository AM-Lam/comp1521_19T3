#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "add.h"

// return the MIPS opcode for add $d, $s, $t
uint32_t add(uint32_t d, uint32_t s, uint32_t t) {
    uint32_t new = 0;
    new |= s << 21;
    new |= t << 16;
    new |= d << 11;
    new |= 1 << 5;
    return new; // REPLACE WITH YOUR CODE

}
