// If you use C library functions add includes here.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "mips_struct.h"

// this is used to separate value of instructions into different bits
mips_structure_t make_bits(uint32_t add) {
    mips_structure_t new = malloc (sizeof (struct mips_structure));
    // this is same as the instruction or whatever value that have passed in
    new->all = add & 0xFFFFFF;
    // opcode: first 6 bits
    new->opcode = (add >> 26) & 63;
    // the following is 5 bits except the last one
    // source code
    new->source1 = (add >> 21) & 31;
    // another source
    new->source2 = (add >> 16) & 31;
    // destination code
    new->dest = (add >> 11) & 31;
    // smaller immediate
    // for "sll" and "srl"
    new->immed = (add >> 6) & 31;
    // last 6 bits
    new->func = add & 63;
    // the mostly used immediate
    new->rest = add & 65535;
    return new;
}
