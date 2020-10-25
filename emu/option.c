#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "mips_struct.h"
#include "option.h"

// In part 1, this is used to print out all the register or value in a correct fomat

// In part 2, this will append the input into the correct value into x, y, z column according to part 1
// Since the same type of command sometimes are using different part of bits, functions will be made in basic and special cases
// and for double digital number, it is more like the branch of single digital number, like Op11 is better called Option 1.1

// print jump command
void Op1(mips_structure_t inp , int mode, uint32_t *X) {
    if (mode == 1) printf("0x%x", inp->all & 0xFFFFFF);
    if (mode == 2) *X = inp->all & 0xFFFFFF;    
}

// print "jr" command
void Op11(mips_structure_t inp , int mode, uint32_t *s) {
    if (mode == 1) printf("$%d", inp->source1);
    if (mode == 2) *s = inp->source1;
}

// print array
void Op2(mips_structure_t inp , int mode, uint32_t *t, uint32_t *O, uint32_t *b) {
    if (mode == 1) printf("$%d, %d($%d)", inp->source2, inp->rest, inp->source1);
    if (mode == 2) {
        *t = inp->source2;
        *O = inp->rest;
        *b = inp->source1;
    }
}

// print branch command
void Op3(mips_structure_t inp , int mode, uint32_t *s, uint32_t *I) {
    if (mode == 2) {
        *s = inp->source1;
        *I = inp->rest;
    }
    if (mode == 1) printf("$%d, %d", inp->source1, twos_complement(inp->rest));
}

// print "lui" command
void Op31(mips_structure_t inp , int mode, uint32_t *t, uint32_t *I) {
    if (mode == 1) printf("$%d, %d", inp->source2, inp->rest);
    if (mode == 2) {
        *t = inp->source2;
        *I = inp->rest;
    }
}

// print register
void Op4(mips_structure_t inp , int mode, uint32_t *d, uint32_t *s, uint32_t *t) {
    if (mode == 1) printf("$%d, $%d, $%d", inp->dest, inp->source1, inp->source2);
    if (mode == 2) {
        *d = inp->dest;
        *s = inp->source1;
        *t = inp->source2;
    }
}

// print set: "sllv" and "srlv" register
void Op41(mips_structure_t inp , int mode, uint32_t *d, uint32_t *t, uint32_t *s) {
    if (mode == 1) printf("$%d, $%d, $%d", inp->dest, inp->source2, inp->source1);
    if (mode == 2) {
        *d = inp->dest;
        *t = inp->source2;
        *s = inp->source1;
    }
}

// print immediate command
void Op5(mips_structure_t inp , int mode, uint32_t *t, uint32_t *s, uint32_t *I) {
    if (mode == 1) printf("$%d, $%d, %d", inp->source2, inp->source1, twos_complement(inp->rest));
    if (mode == 2) {
        *t = inp->source2;
        *s = inp->source1;
        *I = inp->rest; 
    }
}

// print set: "sll" and "srl" immediate
void Op51(mips_structure_t inp , int mode, uint32_t *d, uint32_t *t, uint32_t *I) {
    if (mode == 1) printf("$%d, $%d, %d", inp->dest, inp->source2, inp->immed);
    if (mode == 2) {
        *d = inp->dest;
        *t = inp->source2;
        *I = inp->immed;
    }
}

// print "beq" and "bne" branch 
void Op52(mips_structure_t inp , int mode, uint32_t *s, uint32_t *t, uint32_t *I) {
    if (mode == 2) {
        *s = inp->source1;
        *t = inp->source2;
        *I = inp->rest;
    }
    if (mode == 1) printf("$%d, $%d, %d", inp->source1, inp->source2, inp->rest);
}

int32_t twos_complement(uint32_t num) {
    uint32_t newnum = num;
    uint32_t mask = pow(2, 15);

    return (int32_t)(-(newnum & mask) + (newnum & ~mask));
}
