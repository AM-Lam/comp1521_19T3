#include <stdio.h>
#include <stdint.h>

struct mips_structure {
    uint32_t all;
    uint32_t opcode;
    uint32_t source1;
    uint32_t source2;
    uint32_t dest;
    uint32_t immed;
    uint32_t func;
    uint32_t rest;
};

typedef struct mips_structure *mips_structure_t;
mips_structure_t make_bits(uint32_t);