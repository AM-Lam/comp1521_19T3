// Extract the 3 parts of a float using bit operations only

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

#define EXPONENT_INF_NAN  255

// separate out the 3 components of a float
float_components_t float_bits(uint32_t f) {









}
// // separate out the 3 components of a float
// float_components_t float_bits(uint32_t f) {

//     float_components_t comp;
//     comp.sign = (f >> 31);
//     comp.exponent = (f >> 23) & 0x00FF;
//     comp.fraction = f & 0x7FFFFF;

//     return comp;

// }


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


// given the 3 components of a float
// return 1 if it is 0 or -0, 0 otherwise
int is_zero(float_components_t f) {

    return (f.exponent == 0) && (f.fraction == 0) ? 1 : 0;
}
