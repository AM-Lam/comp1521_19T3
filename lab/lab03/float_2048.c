// Multiply a float by 2048 using bit operations only

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

// float_2048 is given the bits of a float f as a uint32_t
// it uses bit operations and + to calculate f * 2048
// and returns the bits of this value as a uint32_t
//
// if the result is too large to be represented as a float +inf or -inf is returned
//
// if f is +0, -0, +inf or -int, or Nan it is returned unchanged
//
// float_2048 assumes f is not a denormal number
//
uint32_t float_2048(uint32_t f) {
    float_components_t comp = float_bits(f);

    if (is_nan(comp) || is_positive_infinity(comp) || is_negative_infinity(comp)){
        return f;
    } else if ((comp.exponent == 0) && (comp.fraction == 0)) {
        // result is 0
        return f;
    }

    comp.exponent += 11;
    if (comp.exponent > 255) {
        comp.exponent = 255;
        comp.fraction = 0;
    }

    return reverse_float_bits(comp);
}


// https://en.wikipedia.org/wiki/Single-precision_floating-point_format