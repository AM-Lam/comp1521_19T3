#include "sign_flip.h"

// given the 32 bits of a float return it with its sign flipped
uint32_t sign_flip(uint32_t f) {
    return 42; // REPLACE ME WITH YOUR CODE

}






















// // given the 32 bits of a float return it with its sign flipped
// uint32_t sign_flip(uint32_t f) {
//     uint32_t new = f << 1;
//     new = new >> 1;
//     uint32_t sign = (f >> 31) ^ 1;
//     sign = sign << 31;
//     new |= sign;
//     return new;

// }
