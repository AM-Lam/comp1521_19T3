// Sample solution for COMP1521 lab exercises
//
// Print out a float.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

#define EXPONENT_INF_NAN  255

// separate out the 3 components of a float
float_components_t float_bits(uint32_t f) {

    float_components_t comp;
    comp.sign = (f >> 31);
    comp.exponent = (f >> 23) & 0x00FF;
    comp.fraction = f & 0x7FFFFF;

    return comp;

}

// given the 3 components of a float
// return 1 if it is NaN, 0 otherwise
int is_nan(float_components_t f) {
    return (f.fraction != 0) && (f.exponent == EXPONENT_INF_NAN) ? 1 : 0;
}


// given the 3 components of a float
// return 1 if it is inf, 0 otherwise
int is_positive_infinity(float_components_t f) {
    return (f.sign == 0) && (f.exponent == EXPONENT_INF_NAN) && (f.fraction == 0) ? 1 : 0;

}


// given the 3 components of a float
// return 1 if it is -inf, 0 otherwise
int is_negative_infinity(float_components_t f) {
    return (f.sign == 1) && (f.exponent == EXPONENT_INF_NAN) && (f.fraction == 0) ? 1 : 0;
}

uint32_t reverse_float_bits(float_components_t comp) {

    uint32_t num = 0;
    num |= comp.sign << 31;
    num |= (comp.exponent << 23);
    num |= comp.fraction;

    return num;

}

//
// float_print is given the bits of a float as a uint32_t
// it prints out the float in the same format as "%.9g\n"
// using only putchar & puts
//
void float_print(uint32_t bits) {
   float_components_t comp = float_bits(bits);

    if (comp.sign == 1) {
        char *negative = "-";
        puts(negative);
    }

    if (is_nan(comp)){
        puts("nan");
    } else if (is_positive_infinity(comp) || is_negative_infinity(comp)) {
        puts("inf");
    } else if (comp.exponent == 0 && comp.fraction == 0) {
        puts("0");
    } else {

        // uint32_t frac = comp.fraction;
        // int flag = 1;
        // int i = 1;
        // while (flag == 1 && i != 23) {
        //     if (((frac >> i) & 1) != 0) {
        //         flag = 0;
        //     }
        //     i++;
        // }

        uint32_t result = reverse_float_bits(float_bits('0' + bits))
        char *str = result;
        puts(str);
    }

    putchar('\n');
}

