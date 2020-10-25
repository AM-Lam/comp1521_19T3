// Compare 2 floats using bit operations only

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

float_components_t float_bits(uint32_t f) {
    float_components_t comp;
    comp.sign = (f >> 31);
    comp.exponent = (f >> 23) & 0x00FF;
    comp.fraction = f & 0x7FFFFF;
    return comp;
}

int checkNegativity (int one, int two) {
    if (one == two) return 2;
    return (two > one) ? 1 : 0;
}

// float_less is given the bits of 2 floats bits1, bits2 as a uint32_t
// and returns 1 if bits1 < bits2, 0 otherwise
// 0 is return if bits1 or bits2 is Nan
// only bit operations and integer comparisons are used
uint32_t float_less(uint32_t bits1, uint32_t bits2) {
    float_components_t comp1 = float_bits(bits1);
    float_components_t comp2 = float_bits(bits2);

    if ((comp1.exponent == 255 && comp1.fraction != 0) || (comp2.exponent == 255 && comp2.fraction != 0)) return 0;

    if (comp1.sign != comp2.sign) {
        return (comp1.sign == 1) ? 1 : 0;
    } 

    if ((comp1.exponent == 255 && comp2.exponent == 255)) return 0;

    int check = checkNegativity (comp1.exponent, comp2.exponent);
    if (check == 2) {
        check = checkNegativity (comp1.fraction, comp2.fraction);
        if (check == 2) return 0;
    } 

    if (comp1.sign == 1) {
        check = (check == 1) ? 0 : 1;
    }
    
    return check;
}

